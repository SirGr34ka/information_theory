#include "huffman.h"

huffmanAlg::huffmanAlg( std::vector<float>& probs )
{
    tree_tops.resize( probs.size() );
    
    for ( size_t i = 0 ; i < probs.size() ; ++i )
    {
        tree_tops[i] = new treeTop( probs[i] );
    }

    for ( auto& prob: probs )
    {
        codes.insert( std::pair< float , std::string > { prob , {} } );
    }

    code = "";
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
    // ЕСЛИ БУДУТ ОДИНАКОВЫЕ ВЕРОЯТНОСТИ ТО БУДЕТ ГАБЕЛА
    if ( codes.count( top->get_data() ) && top->left == NULL && top->right == NULL )
    {
        codes[ top->get_data() ] = code;

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

    for ( auto& [ key , value ] : codes )
    {
        std::cout << "z" << i++ << ": " << value << " ";
    }

    return;
}

huffmanAlg::~huffmanAlg()
{
    delete tree_tops[0];
}