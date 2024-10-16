#include <vector>
#include <iostream>
#include "tree.h"
#include "huffman.h"

int main(int, char**)
{
    std::vector<float> probs { 0.26 , 0.23 , 0.16 , 0.11 , 0.09 , 0.08 , 0.05 , 0.02 };

    for ( size_t i = 0 ; i < probs.size() ; ++i )
    {
        std::cout << "z" << i << ": " << probs[i] << " ";
    }

    std::cout << std::endl;

    huffmanAlg tree( probs );

    treeTop* top = tree.get_tree();
    top->preorder_output();
    
    std::cout << std::endl;

    return 0;
}