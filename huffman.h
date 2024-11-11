#pragma once

#include <iostream>

#include <vector>
#include <string>
#include <map>
#include <tuple>

#include <algorithm>
#include <cmath>

#include "tree.h"

class huffmanAlg
{
    std::vector<treeTop*> tree_tops;
    std::multimap< float , std::string > codes;

    float entropy;
    float average_length;
    float redundancy;

    /**
     * @brief
     * Строит коды для ансамбля вероятностей
     * 
     * @param top
     * вершина дерева
     * 
     * @param code
     * начальное значение кода
     */
    void get_codes( treeTop* top , std::string code );

    public:

    /**
     * @brief
     * Конструктор класса
     * 
     * @param probs
     * вектор с ансамблем из вероятностей
     */
    huffmanAlg( std::vector< float >& probs );

    /**
     * @brief
     * Строит дерево из ансамбля вероятностей и выводит его
     */
    void output_tree();

    /**
     * @brief
     * Строит коды для ансамбля вероятностей и выводит их
     */
    void output_codes();

    /**
     * @brief
     * Вывод энтропии
     */
    void output_entropy();

    /**
     * @brief
     * Вывод средней длины слова
     */
    void output_average_length();

    /**
     * @brief
     * Вывод избыточности
     */
    void output_redundancy();

    /**
     * @brief
     * Деструктор класса
     */
    ~huffmanAlg();
};