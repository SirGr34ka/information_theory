#include "huffman.h"

huffmanAlg::huffmanAlg( std::vector<float>& probs )
{
    entropy = 0;
    average_length = 0;
    redundancy = 0;

    tree_tops.resize( probs.size() );
    
    for ( size_t i = 0 ; i < probs.size() ; ++i )
    {
        tree_tops[i] = new treeTop( probs[i] );
    }

    for ( auto& prob: probs )
    {
        codes.insert( std::pair< float , std::string > { prob , {} } );
    }
}

void huffmanAlg::output_tree()
{
    // Построение дерева вероятностей
    treeTop* top;
    size_t size;
    
    while ( tree_tops.size() != 1 )
    {
        size = tree_tops.size();

        // Объединение последних двух вероятностей в одну
        top = new treeTop( tree_tops[ size - 1 ]->get_data() + tree_tops[ size - 2 ]->get_data() );
        top->push_right( tree_tops[ size - 1 ] );
        top->push_left( tree_tops[ size - 2 ] );

        tree_tops.pop_back();
        tree_tops.pop_back();
        tree_tops.push_back( top );

        // Сортировка по вероятностям после объединения
        std::sort( tree_tops.begin() , tree_tops.end() , []( treeTop* a , treeTop* b ){ return a->get_data() > b->get_data(); });
    }

    // Вывод дерева вероятностей
    tree_tops[0]->preorder_output();

    std::cout << std::endl;

    return;
}

void huffmanAlg::get_codes( treeTop* top , std::string code )
{
    float prob = top->get_data();

    if ( codes.count( prob ) && top->left == NULL && top->right == NULL )
    {
        // Если есть одинаковые вероятности в ансамбле
        if( codes.count( prob ) > 1 )
        {
            auto iters = codes.equal_range( prob );

            for ( auto pair = std::get<0>(iters) ; pair != std::get<1>(iters) ; ++pair )
            {
                if( std::get<1>(*pair) == "" )
                {
                    std::get<1>(*pair) = code;
                    break;
                }
            }
        }
        else
        {
            std::get<1>(*codes.find(prob)) = code;
        }

        return;
    }

    if ( top->left != NULL )
    {
        get_codes( top->left , code + "1" );
    }

    if ( top->right != NULL )
    {
        get_codes( top->right , code + "0" );
    }

    return;
}

void huffmanAlg::output_codes()
{
    get_codes( tree_tops[0] , "" );

    size_t i = 0;

    for ( auto& pair : codes )
    {
        std::cout << "z" << ++i << ": " << pair.second << " ";
    }

    std::cout << std::endl;

    return;
}

void huffmanAlg::output_entropy()
{
    for ( auto& pair : codes )
    {
        entropy += pair.first * log2f( pair.first );
    }

    entropy = -entropy;

    std::cout << "Entropy: " << entropy << std::endl;
}

void huffmanAlg::output_average_length()
{
    for ( auto& pair : codes )
    {
        average_length += pair.first * pair.second.size();
    }

    std::cout << "Average length: " << average_length << std::endl;
}

void huffmanAlg::output_redundancy()
{
    redundancy = 1 + ( (entropy) / ( log2f( ( float )1 / codes.size() ) ) );

    std::cout << "Readundancy: " << redundancy << std::endl;
}

huffmanAlg::~huffmanAlg()
{
    delete tree_tops[0];
}