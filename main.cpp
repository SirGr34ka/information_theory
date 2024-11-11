#include <iostream>

#include <vector>

#include "huffman.h"

int main(int, char**)
{
    // Вывод вероятностей
    std::vector<float> probs { 0.26F , 0.23F , 0.16F , 0.11F , 0.09F , 0.08F , 0.05F , 0.02F };

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

    // Вывод энтропии, средней длины слова и избыточность
    tree.output_entropy();
    tree.output_average_length();
    tree.output_redundancy();

    return 0;
}