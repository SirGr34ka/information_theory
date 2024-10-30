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
    std::vector<float> probs;

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
     * Строит дерево из ансамбля
     */
    treeTop* get_tree();

    std::map< float , std::string > get_codes();

    /**
     * @brief
     * Деструктор класса
     */
    ~huffmanAlg();
};