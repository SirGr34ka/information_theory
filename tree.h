#pragma once

class treeTop
{
    float data;

    treeTop* right;
    treeTop* left;

    public:

    /**
     * @brief
     * Конструктор класса
     * 
     * @param data
     * данные
     */
    treeTop( const float _data );

    void push_right( treeTop* top );
    void push_left( treeTop* top );

    /**
     * @brief
     * Деструктор класса
     */
    ~treeTop();
};