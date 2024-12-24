#include <iostream>

#include <vector>
#include <string>
#include <tuple>

#include <algorithm>
#include <cmath>

// Шаблоны функции
template < typename T >
int find_character( const std::vector< std::pair< std::string , T > >& charset , const std::string character );

void sort_codes_by_charset( const std::vector< std::pair< std::string , float > >* charset_ptr,
                            std::vector< std::pair< std::string , std::string > >& codes );
//-----------------------------------------------------------------------------------------------------------------
// Класс ноды дерева
template < class T >
class TreeTop
{
    T data;

    public:

    TreeTop* right;
    TreeTop* left;

    // Конструктор класса
    TreeTop( const T& _data )
    {
        data = _data;

        left = NULL;
        right = NULL;
    }

    // Гетер данных из вершины
    T& get_data()
    {
        return data;
    }

    // Пушит вправо от вершины
    void push_right( TreeTop* top )
    {
        right = top;

        return; 
    }

    // Пушит влево от вершины
    void push_left( TreeTop* top )
    {
        left = top;

        return;
    }

    // Префиксный обход дерева
    /*
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
    */

    // Деструктор класса
    ~TreeTop()
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
class HuffmanAlg
{
    const std::vector< std::pair< std::string , float > >* charset_ptr;
    std::vector< std::pair< std::string , std::string > > codes;

    // Вектор указателей на вершину дерева, которая хранит данные вида: std::pair< буква , вероятность >
    std::vector<TreeTop< std::pair< std::string , float > >* > tree_tops;

    float entropy;
    float average_length;
    float redundancy;

    // Строит дерево из ансамбля букв и их вероятностей
    void build_tree()
    {
        TreeTop< std::pair< std::string , float > >* top;
        size_t size;
        
        // Пока не получим корень дерева
        while ( tree_tops.size() != 1 )
        {
            size = tree_tops.size();

            // Объединение последних двух вероятностей в одну
            top = new TreeTop< std::pair< std::string , float > >( { "",        // Пустая буква
                                tree_tops[ size - 1 ]->get_data().second +      // Вероятность последней вершины в tree_tops
                                tree_tops[ size - 2 ]->get_data().second } );   // Вероятность предпоследней вершины в tree_tops
            top->push_right( tree_tops[ size - 1 ] );
            top->push_left( tree_tops[ size - 2 ] );

            tree_tops.pop_back();
            tree_tops.pop_back();
            tree_tops.push_back( top );

            // Сортировка с помощью метода std::sort по вероятностям после объединения
            std::sort(  tree_tops.begin(),                                      // Итератор на начало tree_tops
                        tree_tops.end(),                                        // Итератор на конец tree_tops
                        []( TreeTop< std::pair< std::string , float > >* a,     // Лямбда функция сравнения двух вершин по их вероятностям
                            TreeTop< std::pair< std::string , float > >* b )
            { return a->get_data().second > b->get_data().second; });           // Возвращаем результат сравнения вторых элементов пар, то есть вероятностей
        }

        // Вывод дерева вероятностей
        /*
        tree_tops[0]->preorder_output();

        std::cout << std::endl;
        */

        return;
    }

    // Строит коды для ансамбля вероятностей
    void get_codes( TreeTop< std::pair< std::string , float > >* top , std::string code )
    {
        // Если буква в вершине не равна пустой строке
        if ( top->get_data().first != "" )
        {
            // Добавление кода в формате: { буква , код }
            codes.push_back( { top->get_data().first , code } );

            return;
        }

        get_codes( top->left , code + "1" );
        get_codes( top->right , code + "0" );

        return;
    }

    public:

    // Конструктор класса, принимающий указатель на ансамбль букв и их вероятностей
    HuffmanAlg( const std::vector< std::pair< std::string , float > >* charset_ptr_ )
    {
        entropy = 0;
        average_length = 0;
        redundancy = 0;

        charset_ptr = charset_ptr_;

        tree_tops.reserve( charset_ptr_->size() );
        
        for ( size_t i = 0 ; i < charset_ptr_->size() ; ++i )
        {
            tree_tops.push_back( new TreeTop< std::pair< std::string , float > >( (*charset_ptr_)[i] ));
        }

        codes.reserve( charset_ptr_->size() );
    }

    // Строит коды для ансамбля вероятностей и выводит их
    void output_codes()
    {
        // Находим корень дерева
        build_tree();

        // Нахождение кодов
        get_codes( tree_tops[0] , "" );

        // Сортировка codes
        sort_codes_by_charset( charset_ptr , codes );
        
        // Вывод кодов
        for ( auto iter = codes.cbegin() ; iter != codes.cend() ; ++iter )
        {
            std::cout << iter->first << ": " << iter->second << std::endl;
        };
    }

    // Вывод энтропии
    // Вывод энтропии
    void output_entropy()
    {
        for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
        {
            entropy += (*charset_ptr)[i].second * log2f( (*charset_ptr)[i].second );
        }

        entropy = -entropy;

        std::cout << "Huffman algorithm entropy: " << entropy << std::endl;
    }

    // Вывод средней длины слова
    void output_average_length()
    {
        for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
        {
            average_length += (*charset_ptr)[i].second * codes[i].second.size();
        }

        std::cout << "Huffman algorithm average length: " << average_length << std::endl;
    }

    // Вывод избыточности
    void output_redundancy()
    {
        redundancy = 1 + ( (entropy) / ( log2f( ( float )1 / codes.size() ) ) );

        std::cout << "Huffman algorithm readundancy: " << redundancy << std::endl;
    }

    // Деструктор класса
    ~HuffmanAlg()
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

        float differential{};
        float min_differential{};

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
        sort_codes_by_charset( charset_ptr , codes );
        
        // Вывод кодов
        for ( auto iter = codes.cbegin() ; iter != codes.cend() ; ++iter )
        {
            std::cout << iter->first << ": " << iter->second << std::endl;
        }

        return;
    }

    // Вывод энтропии
    void output_entropy()
    {
        for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
        {
            entropy += (*charset_ptr)[i].second * log2f( (*charset_ptr)[i].second );
        }

        entropy = -entropy;

        std::cout << "Shannon-Fano algorithm entropy: " << entropy << std::endl;
    }

    // Вывод средней длины слова
    void output_average_length()
    {
        for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
        {
            average_length += (*charset_ptr)[i].second * codes[i].second.size();
        }

        std::cout << "Shannon-Fano algorithm average length: " << average_length << std::endl;
    }

    // Вывод избыточности
    void output_redundancy()
    {
        redundancy = 1 + ( (entropy) / ( log2f( ( float )1 / codes.size() ) ) );

        std::cout << "Shannon-Fano algorithm readundancy: " << redundancy << std::endl;
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

// Сортировка букв в codes по расположению букв в charset
void sort_codes_by_charset( const std::vector< std::pair< std::string , float > >* charset_ptr, // Указатель на charset
                            std::vector< std::pair< std::string , std::string > >& codes )      // Ссылка на codes
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

void generate_combinations( const std::vector< std::pair< std::string , float > >& charset , const size_t COMB_LENGTH,
                     std::vector< std::pair< std::string , float > >& combset )
{
    size_t n = charset.size();  // Количество символов
    std::vector< size_t > indices( COMB_LENGTH , 0 );  // Индексы для генерации комбинаций

    do {
        std::string comb = "";  // Блок символов
        float prob = 1.0F;  // Вероятность блока

        // Создаем блок из символов и считаем его вероятность
        for ( size_t i = 0; i < COMB_LENGTH ; ++i )
        {
            comb += charset[ indices[i] ].first;   // Формируем строку блока
            prob *= charset[ indices[i] ].second;  // Умножаем вероятности символов
        }

        // Добавляем блок в список
        combset.push_back( { comb , prob } );
    } 
    
    // [&]: Захват переменных. & означает, что переменные из внешней области видимости будут захвачены по ссылке.
    // (): Пустые круглые скобки означают, что функция не принимает параметров.
    // { ... }: Тело лямбда-функции.
    // () после тела функции: Немедленный вызов лямбда-функции.
    while ([&]()
    {
        // Генерация всех комбинаций с помощью счетчика
        for ( int i = (int)COMB_LENGTH - 1 ; i >= 0 ; --i )
        {
            if ( ++indices[i] < n ) // Увеличиваем текущий индекс
            {
                return true;
            }

            indices[i] = 0;  // Сбрасываем индекс, если достигнут предел
        }

        return false;  // Все комбинации сгенерированы
    }());
}

//---------------------------------------------------------------------------------------------------------
int main(int, char**)
{
    const std::vector< std::pair< std::string , float > > charset {
        { "a" , 0.7F },
        { "b" , 0.3F }
    };

    const size_t COMB_LENGTH = 3; // Длина комбинации

    std::vector< std::pair< std::string , float > > combset; // Ансамбль комбинаций и их вероятностей

    combset.reserve( (size_t)std::powf( (float)COMB_LENGTH , (float)charset.size() ) ); // Резервирование памяти под комбинации

    generate_combinations( charset , COMB_LENGTH , combset ); // Заполнение вектора комбинациями

    std::sort( combset.begin() , combset.end(),
    // Лямбда функция для сортировки по значению вероятности
    []( std::pair< std::string , float > a , std::pair< std::string , float > b )
    {
        return a.second > b.second; // Сравнение вероятностей
    } );

    // Вывод ансамбля букв и их вероятностей
    std::cout << "Charset:" << std::endl;

    for ( auto iter = charset.cbegin() ; iter != charset.cend() ; ++iter )
    {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }

    // Вывод комбинаций и их вероятностей
    std::cout << "Combset:" << std::endl;

    for ( auto iter = combset.cbegin() ; iter != combset.cend() ; ++iter )
    {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }

    HuffmanAlg huffman_alg ( &combset );
    ShannonFanoAlg shannon_fano_alg ( &combset );

    std::cout << "Huffman codes:" << std::endl;

    huffman_alg.output_codes();

    std::cout << "Shannon-Fano codes:" << std::endl;

    shannon_fano_alg.output_codes();

    huffman_alg.output_average_length();
    shannon_fano_alg.output_average_length();

    return 0;
}