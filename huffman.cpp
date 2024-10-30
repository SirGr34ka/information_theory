#include "huffman.h"

huffmanAlg::huffmanAlg( std::vector<float>& _probs )
{
    probs = _probs;
    tree_tops.resize( _probs.size() );
    
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

        std::sort( tree_tops.begin() , tree_tops.end() , []( treeTop* a , treeTop* b ){ return a->get_data() < b->get_data(); });
    }

    return tree_tops[0];
}

huffmanAlg::~huffmanAlg()
{
    delete tree_tops[0];
}