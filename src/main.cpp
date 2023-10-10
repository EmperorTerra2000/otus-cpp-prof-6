/*!
\file
\brief Файл main

В файле выполняются следующие пункты ДЗ 6:

- При запуске программы необходимо создать матрицу с пустым значением 0, заполнить главную
диагональ матрицы (от [0,0] до [9,9]) значениями от 0 до 9.
- Второстепенную диагональ (от [0,9] до [9,0]) значениями от 9 до 0.
- Необходимо вывести фрагмент матрицы от [1,1] до [8,8]. Между столбцами пробел. Каждая строка
матрицы на новой строке консоли.
- Вывести количество занятых ячеек.
- Вывести все занятые ячейки вместе со своими позициями.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <map>

#include "matrix.hpp"

int main(int argc, char *argv[])
{
    Matrix<int, 0> matrix;

    // заполнение основной диагонали матрицы
    for (int i = 0; i < 10; i++)
    {
        
        matrix[i][i] = i;
    }

    // заполнение второстепенной диагонали матрицы
    for (int i = 0; i < 10; i++)
    {
        matrix[i][9 - i] = 9 - i;
    }

    /*
        вывод в консоль элементы матрицы
        от [1, 1] до [8, 8]
    */
    for (int i = 1; i < 9; i++)
    {
        for (int j = 1; j < 9; j++)
        {
            std::cout << matrix[i][j] << " ";
        }

        std::cout << std::endl;
    }

    // количество занятых ячеек
    std::cout << "size matrix => " << matrix.size() << std::endl;

    // вывод элементов с их позицией в матрице
    for (auto el : matrix)
    {
        int row{};
        int column{};
        int data{};

        std::tie(row, column, data) = el;

        std::cout << "[" << row << "]"
                  << "[" << column << "]"
                  << " = " << data << std::endl;
    }

    return 0;
}