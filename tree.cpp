#include <iostream>

#include "tree.h"

treeTop::treeTop( const float _data )
{
    data = _data;

    left = NULL;
    right = NULL;
}

float treeTop::get_data()
{
    return data;
}

void treeTop::push_right( treeTop* top )
{
    right = top;

    return;
}

void treeTop::push_left( treeTop* top )
{
    left = top;

    return;
}

void treeTop::preorder_output()
{
    if (this == NULL)
    {
        return;
    }

    std::cout << this->get_data() << " ";
    left->preorder_output();
    right->preorder_output();

    return;
}

treeTop::~treeTop()
{
    if ( right != NULL )
    {
        delete right;
    }
    
    if ( left != NULL )
    {
        delete left;
    }
}