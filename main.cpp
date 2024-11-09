#include <vector>
#include <iostream>
#include "tree.h"
#include "huffman.h"

int main(int, char**)
{
    // Вывод вероятностей
    std::vector<float> probs { 0.26 , 0.23 , 0.16 , 0.11 , 0.09 , 0.08 , 0.05 , 0.02 };

    for ( size_t i = 1 ; i < probs.size() ; ++i )
    {
        std::cout << "z" << i << ": " << probs[i] << " ";
    }

    std::cout << std::endl;

    // Вывод префиксного обхода дерева
    huffmanAlg tree( probs );

    tree.output_tree();

    // Вывод кодов
    tree.output_codes();

    std::cout << std::endl;

    return 0;
}