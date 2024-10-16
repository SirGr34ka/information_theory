#include "huffman.h"

huffmanAlg::huffmanAlg( std::vector<float>& probs )
{
    tree_tops.resize( probs.size() );
    
    for ( size_t i = 0 ; i < probs.size() ; ++i )
    {
        tree_tops[i] = new treeTop( probs[i] );
    }
}

treeTop* huffmanAlg::get_tree()
{
    treeTop* top;
    size_t size;
    
    while ( tree_tops.size() != 1 )
    {
        size = tree_tops.size();

        top = new treeTop( tree_tops[ size - 1 ]->get_data() + tree_tops[ size - 2 ]->get_data() );
        top->push_right( tree_tops[ size - 1 ] );
        top->push_left( tree_tops[ size - 2 ] );

        tree_tops.pop_back();
        tree_tops.pop_back();
        tree_tops.push_back( top );

        tree_tops_sort();
    }

    return tree_tops[0];
}

void huffmanAlg::tree_tops_sort()
{
    treeTop* temp;

    for ( size_t i = 0 ; i < ( tree_tops.size() - 1 ) ; ++i )
    {
        for ( size_t j = i ; j < tree_tops.size() ; ++j )
        {
            if ( tree_tops[j]->get_data() < tree_tops[ j + 1 ]->get_data() )
            {
                temp = tree_tops[j];
                tree_tops[j] = tree_tops[ j + 1 ];
                tree_tops[ j + 1 ] = temp;
            }
        }
    }

    return;
}

huffmanAlg::~huffmanAlg()
{
    delete tree_tops[0];
}