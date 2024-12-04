#include <iostream>

#include <vector>
#include <string>
#include <map>
#include <tuple>

#include <algorithm>
#include <cmath>

// Класс ноды дерева
class treeTop
{
    float data;

    public:

    treeTop* right;
    treeTop* left;

    // Конструктор класса
    treeTop( const float _data )
    {
        data = _data;

        left = NULL;
        right = NULL;
    }

    // Гетер данных из вершины
    float get_data()
    {
        return data;
    }

    // Пушит вправо от вершины
    void push_right( treeTop* top )
    {
        right = top;

        return; 
    }

    // Пушит влево от вершины
    void push_left( treeTop* top )
    {
        left = top;

        return;
    }

    // Префиксный обход дерева
    void preorder_output()
    {
        std::cout << this->get_data() << " ";

        if ( left != NULL )
        {
            left->preorder_output();
        }

        if ( right != NULL )
        {
            right->preorder_output();
        }

        return;
    }

    // Деструктор класса
    ~treeTop()
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
};

class huffmanAlg
{
    std::vector<treeTop*> tree_tops;
    std::multimap< float , std::string > codes;

    float entropy;
    float average_length;
    float redundancy;

    // Строит коды для ансамбля вероятностей
    void get_codes( treeTop* top , std::string code )
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

    public:

    // Конструктор класса
    huffmanAlg( std::vector< float >& probs )
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

    // Строит дерево из ансамбля вероятностей и выводит его
    void output_tree()
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

    // Строит коды для ансамбля вероятностей и выводит их
    void output_codes()
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

    // Вывод энтропии
    void output_entropy()
    {
        for ( auto& pair : codes )
        {
            entropy += pair.first * log2f( pair.first );
        }

        entropy = -entropy;

        std::cout << "Entropy: " << entropy << std::endl;
    }

    // Вывод средней длины слова
    void output_average_length()
    {
        for ( auto& pair : codes )
        {
            average_length += pair.first * pair.second.size();
        }

        std::cout << "Average length: " << average_length << std::endl;
    }

    // Вывод избыточности
    void output_redundancy()
    {
        redundancy = 1 + ( (entropy) / ( log2f( ( float )1 / codes.size() ) ) );

        std::cout << "Readundancy: " << redundancy << std::endl;
    }

    // Деструктор класса
    ~huffmanAlg()
    {
        delete tree_tops[0];
    }
};

int main(int, char**)
{
    // Вывод вероятностей
    std::vector<float> probs { 0.21F , 0.19F , 0.15F , 0.13F , 0.12F , 0.09F , 0.06F , 0.05F };

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