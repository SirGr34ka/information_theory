#include <iostream>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <tuple>

#include <algorithm>
#include <cmath>

template < typename T >
int find_character( const std::vector< std::pair< std::string , T > >& charset , const std::string character );

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
//---------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------
class ShannonFanoAlg
{
    const std::vector< std::pair< std::string , float > >* charset_ptr;
    std::vector< std::pair< std::string , std::string > > codes;

    float entropy;
    float average_length;
    float redundancy;

    // Заполнение ансамбля буквами и их кодами
    void get_codes(
        std::vector< std::pair< std::string , float > >::const_iterator begin, // Итератор на начало блока
        std::vector< std::pair< std::string , float > >::const_iterator end,  // Итератор на конец блока
        std::string code )
    {
        // Если остался один элемент
        if ( begin + 1 == end )
        {
            // Добавление кода в формате: { буква , код }
            codes.push_back( { begin->first , code } );

            return;
        }

        auto iter = begin + 1; // Итератор на последующий после начала блока элемент

        float differential;
        float min_differential;

        // Поиск элемента при котором, разница разбиения блока на подблоки будет минимальной
        for ( ; iter != end ; ++iter )
        {
            // Вычисление разницы между блоками
            differential = fabs( sum( begin , iter ) - sum( iter , end ) );

            // Если минимальная разница меньше найденной, то цикл нужно прекратить
            if ( ( min_differential < differential ) && ( iter != begin + 1 ) )
            {
                break;
            }

            min_differential = differential;
        }

        get_codes( begin , iter - 1 , code + "1" ); // Рекурсивный вызов этой же функции для левого подблока
        get_codes( iter - 1 , end , code + "0" ); // Рекурсивный вызов этой же функции для правого подблока

        return;
    }

    // Сумма вероятностей ансамбля определенного диапазона (нужна для функции get_codes)
    float sum(
        std::vector< std::pair< std::string , float > >::const_iterator begin, // Итератор на начало диапазона
        std::vector< std::pair< std::string , float > >::const_iterator end ) // Итератор на конец диапазона
    {
        float result = 0;

        while ( begin != end )
        {
            // Прибавление вероятности буквы
            result += begin->second;

            ++begin;
        }

        return result;
    }

    // Сортировка букв в codes по расположению букв в charset
    void sort_codes_by_charset()
    {
        std::pair< std::string , std::string > temp;
        int index;

        for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
        {
            // Поиск буквы в codes ( < Тип шаблона >, соответствено float для charset и std::string для codes  )
            index = find_character< std::string >( codes , (*charset_ptr)[i].first );

            // Если буква не найдена
            if ( index < 0 )
            {
                throw "Not found!";
            }

            // Если буква в codes не на своем месте
            if ( i != (size_t)index )
            {
                temp = codes[i];
                codes[i] = codes[index];
                codes[index] = temp;
            }
        }

        return;
    }

    public:

    // Конструктор класса, принимающий указатель на ансамбль букв и их вероятностей
    ShannonFanoAlg( const std::vector< std::pair< std::string , float > >* charset_ptr_ )
    {
        entropy = 0;
        average_length = 0;
        redundancy = 0;

        // Сохранение указателя на ансамбль
        charset_ptr = charset_ptr_;

        // Расширение памяти под количество букв из ансамбля
        codes.reserve( charset_ptr_->size() );
    }

    // Вывод ансамбля букв и их кодов
    void output_codes()
    {
        auto begin = charset_ptr->cbegin();
        auto end = charset_ptr->cend();

        // Нахождение кодов
        get_codes( begin , end , "" );

        // Сортировка codes
        sort_codes_by_charset();
        
        // Вывод кодов
        for ( auto iter = codes.cbegin() ; iter != codes.cend() ; ++iter )
        {
            std::cout << iter->first << ": " << iter->second << std::endl;
        }

        return;
    }
};

// Поиск буквы в charset или codes
template < typename T >
int find_character( const std::vector< std::pair< std::string , T > >& charset , const std::string character )
{
    for ( size_t i = 0 ; i < charset.size() ; ++i )
    {
        // Если буква равна искомой
        if ( charset[i].first == character )
        {
            return (int)i;
        }
    }

    return -1;
}

//---------------------------------------------------------------------------------------------------------
int main(int, char**)
{
    const std::vector< std::pair< std::string , float > > charset {
        { "z1" , 0.21F },
        { "z2" , 0.19F },
        { "z3" , 0.15F },
        { "z4" , 0.13F },
        { "z5" , 0.12F },
        { "z6" , 0.09F },
        { "z7" , 0.06F },
        { "z8" , 0.05F }
    };

    // Вывод ансамбля букв и их вероятностей
    for ( auto iter = charset.cbegin() ; iter != charset.cend() ; ++iter )
    {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }

    ShannonFanoAlg alg ( &charset );

    alg.output_codes();

    return 0;
}