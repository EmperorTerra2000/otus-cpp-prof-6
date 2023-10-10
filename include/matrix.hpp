/*!
\file
\brief Файл matrix

Присутствуют объявления классов
Matrix, Access, DepthAccess, Node, Iterator
*/

/*!
  \brief Класс Матрица

  Класс 2-мерной разреженной бесконечной матрицы, заполненная значениями по
  умолчанию. Матрица должна хранить только занятые элементы - значения которых хотя бы раз
  присваивались. Присвоение в ячейку значения по умолчанию освобождает ячейку.
*/
template <class T, int DEFAULT_NUMBER = -1>
class Matrix
{
public:
    Matrix()
    {
        access = new Access(dataArr);

        dataArr.reserve(100);
    }

    ~Matrix()
    {
        delete access;
    }

    Matrix(Matrix const &other) = default;

    Matrix &operator=(Matrix const &other) = default;

    /*!
      \brief Класс Node

      Структура, в котором размещаются строка, столбец
      и значение элемента матрицы
    */
    template <class U>
    struct Node
    {
        int row{};    ///< значение строки
        int column{}; ///< значение таблицы
        U data{};     ///< значение элемента матрицы
    };

    /*!
      Возвращает кол-во заполненных ячеек матрицы

      \return кол-во элементов
    */
    int size()
    {
        return dataArr.size();
    }

    /*!
      \brief Класс DepthAccess

      Этот класс нужен для дальнейшего доступа по цепочке к элементу матрицы.
      Вызывается из перегруженного оператора []
    */
    class DepthAccess
    {
    public:
        DepthAccess(std::vector<Node<T> *> &ptr) : pDataArr(ptr){};

        /*!
          Поиск элемента в векторе std::vector<Node<T> *>

          \return итератор на найденный элемент или итератор на конец
        */
        auto findElement()
        {
            return std::find_if(pDataArr.cbegin(), pDataArr.cend(), [&](Node<T> *el)
                                { return (el->row == _row && el->column == _column); });
        }

        /*!
          Поиск элемента в векторе std::vector<Node<T> *>

          \param val переменная справа от оператора =
          \return итератор на найденный элемент или итератор на конец
        */
        DepthAccess &operator=(const T val)
        {
            // ищем элемент
            auto ItElement = findElement();

            // если он уже существует
            if (ItElement != std::end(pDataArr))
            {
                // то переопределяем только величину data
                (*ItElement)->data = val;
            }
            else
            {
                // иначе создаем Node и пушим в вектор
                Node<T> *node = new Node<T>{_row, _column, val};

                pDataArr.push_back(std::move(node));
            }

            return *this;
        }

        /*!
          Переопределение оператора <<

          \param stream ссылка на потоковый объект
          \param element ссылка на элемент находящийся слева от оператора <<
          \return ссылку на объект потока
        */
        friend std::ostream &operator<<(std::ostream &stream, DepthAccess &element)
        {
            stream << element.printElement();

            return stream;
        }

        /*!
          Возвращает значение в зависимости от его существования в векторе для
          дальнейшей печати этого значения в стандартный поток вывода

          \return число присутствующий в векторе или -1 (DEFAULT_NUMBER)
        */
        int printElement()
        {
            // поиск элемента
            auto ItElement = findElement();

            if (ItElement != std::end(pDataArr))
            {
                return (*ItElement)->data;
            }
            else
            {
                return DEFAULT_NUMBER;
            }
        }

        /*!
          Установка строки и столбца

          \param row строка
          \param column столбец
        */
        void setRowAndColumn(int row, int column)
        {
            _row = row;
            _column = column;
        }

    private:
        std::vector<Node<T> *> &pDataArr;
        int _row{}, _column{};
    };

    /*!
      \brief Класс Access

      Этот класс нужен для дальнейшего доступа по цепочке к элементу матрицы.
      Вызывается из перегруженного оператора []
    */
    class Access
    {
    public:
        Access(std::vector<Node<T> *> &ptr) : pDataArr(ptr)
        {
            depthAccess = new DepthAccess(ptr);
        };

        ~Access()
        {
            delete depthAccess;
        }

        Access &operator=(Access const &other) = default;

        /*!
          Устанавливается значение строки

          \param val значение строки
        */
        void setRow(int val)
        {
            row = val;
        }

        /*!
          Перегруженный оператор []

          \param column значение столбца
          \return ссылку на объект DepthAccess для дальнейшей передачи управления
        */
        DepthAccess &operator[](int column)
        {
            depthAccess->setRowAndColumn(row, column);

            return *depthAccess;
        }

    private:
        int row;

        DepthAccess *depthAccess;
        std::vector<Node<T> *> &pDataArr;
    };

    /*!
      Перегруженный оператор []

      \param val значение строки
      \return ссылку на объект Access для дальнейшей передачи управления
    */
    Access &operator[](const int &val)
    {
        access->setRow(val);

        return *access;
    }

    class Iterator
    {
    public:
        explicit Iterator(const std::vector<Node<T> *> &ptr, int index_ = 0)
            : pDataArr(ptr),
              index(index_) {}

        /*!
          Перегруженный оператор инкрементации

          \return ссылку на себя
        */
        Iterator &operator++()
        {
            index++;
            return *this;
        }

        /*!
          Перегруженный оператор разыменования

          \return объект std::make_tuple
        */
        std::tuple<int, int, int> operator*() const
        {
            return std::make_tuple(pDataArr[index]->row, pDataArr[index]->column, pDataArr[index]->data);
        }

        /*!
          Перегруженный оператора "не равно"
          будет происходить сравнение с итератором end
          
          \param other ссылка на другой итератор
          \return сравнение итераторов begin и end
        */
        bool operator!=(const Iterator &other)
        {
            return index != other.index;
        }

    private:
        const std::vector<Node<T> *> &pDataArr;
        int index{};
    };

    /*!
      Внутри метода begin создается объект класса Iterator
      в качестве параметра которого передается ссылка на
      вектор std::vector<Node<T> *> элементов матрицы

      \return объект класса Iterator
    */
    Iterator begin() const
    {
        return Iterator(dataArr);
    }

    /*!
      Внутри метода end создается объект класса Iterator
      в качестве параметра которого передается ссылка на
      вектор std::vector<Node<T> *> элементов матрицы и
      размер этого вектора

      \return объект класса Iterator
    */
    Iterator end() const
    {
        return Iterator(dataArr, dataArr.size());
    }

private:
    std::vector<Node<T> *> dataArr; ///< вектор из элементов матрицы
    Access *access;                 ///< указатель на объект доступа
};