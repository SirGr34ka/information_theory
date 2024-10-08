#include <vector>
#include <iostream>
#include "tree.h"

void sort ( std::vector<float>& arr )
{
    float temp;

    for ( size_t i = 0 ; i < ( arr.size() - 1 ) ; ++i )
    {
        for ( size_t j = 1 ; j < arr.size() ; ++j )
        {
            if ( arr[i] < arr[j] )
            {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

float sum ( std::vector<float>& arr )
{
    float summary = 0;

    for ( float num: arr )
    {
        summary += num;
    }

    return summary;
}

int main(int, char**)
{
    std::vector<float> probs { 0.26 , 0.23 , 0.16 , 0.11 , 0.09 , 0.08 , 0.05 , 0.02 };

    for ( size_t i = 0 ; i < probs.size() ; ++i )
    {
        std::cout << "z" << i << ": " << probs[i] << " ";
    }

    std::cout << std::endl;

    treeTop* right = NULL;
    treeTop* left = NULL;
    treeTop* top = NULL;

    while ( int( probs[0] ) != 1 )
    {
        float right_prob = probs[ probs.size() - 1 ];
        probs.pop_back();
        right = new treeTop( right_prob );

        float left_prob = probs[ probs.size() - 1 ];
        probs.pop_back();
        left = new treeTop( left_prob );

        float sum_prob = right_prob + left_prob;
        probs.push_back( sum_prob );
        top = new treeTop( sum_prob );
        top->push_right( right );
        top->push_left( left );

        sort( probs );
    }

    return 0;
}