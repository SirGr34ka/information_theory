#pragma once;

#include <vector>
#include <iostream>
#include "tree.h"

class huffmanAlg
{
    std::vector<treeTop*> tree_tops;

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
    
    /**
     * @brief 
     * Сортировка вершин по их значениям
     */
    void tree_tops_sort();

    /**
     * @brief
     * Деструктор класса
     */
    ~huffmanAlg();
};