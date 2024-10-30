#pragma once;

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

#include "tree.h"

class huffmanAlg
{
    std::vector<treeTop*> tree_tops;
    std::map< float , std::string > codes;
    std::string code;

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

    void output_codes();

    /**
     * @brief
     * Деструктор класса
     */
    ~huffmanAlg();
};