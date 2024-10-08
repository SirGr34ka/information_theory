#include <iostream>

#include "tree.h"

treeTop::treeTop( const float _data )
{
    data = _data;

    left = NULL;
    right = NULL;
}

void treeTop::push_right( treeTop* top )
{
    right = top;
}

void treeTop::push_left( treeTop* top )
{
    left = top;
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