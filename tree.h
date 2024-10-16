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

    /**
     * @brief
     * Гетер данных из вершины
     * 
     * @return
     * Возвращает данные вершины
     */
    float get_data();

    /**
     * @brief
     * Пушит вправо от вершины
     * 
     * @param top
     * Вершина, которую нужно запушить
     */
    void push_right( treeTop* top );

    /**
     * @brief
     * Пушит влево от вершины
     * 
     * @param top
     * Вершина, которую нужно запушить
     */
    void push_left( treeTop* top );

    /**
     * @brief
     * Префиксный обход дерева
     */
    void preorder_output();

    /**
     * @brief
     * Деструктор класса
     */
    ~treeTop();
};