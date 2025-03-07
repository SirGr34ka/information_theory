#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <tuple>

#include <algorithm>
#include <cmath>
#include <bitset>

size_t COMB_LENGTH = 5; // Длина комбинации

// Шаблоны функции
template < typename T >
int find_character( const std::vector< std::pair< std::string , T > >& charset , const std::string& character );

int find_code( const std::vector< std::pair< std::string , std::string > >& codes , const std::string& code );

void sort_codes_by_charset( const std::vector< std::pair< std::string , float > >* charset_ptr,
                            std::vector< std::pair< std::string , std::string > >& codes );
//-----------------------------------------------------------------------------------------------------------------
// Класс ноды дерева
template < class T >
struct TreeTop
{
    T data;

    TreeTop* right;
    TreeTop* left;
    TreeTop* previous;

    // Конструктор класса
    TreeTop( const T& _data )
    {
        data = _data;

        left = nullptr;
        right = nullptr;
        previous = nullptr;
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
    // Для метода archive в HuffmanAlg
    void preorder_output(std::string& combs , // Последовательность комбинаций
                                std::string& tree )  // Дерево в побитовом представлении
    {
        if ( left != nullptr )
        {
            tree += "1";
            left->preorder_output( combs , tree );
            tree += "0";
        }

        if ( right != nullptr )
        {
            tree += "1";
            right->preorder_output( combs , tree );
            tree += "0";
        }

        if ( data.first != "" )
        {
            combs += data.first;
        }

        return;
    }

    // Деструктор класса
    ~TreeTop()
    {
        if ( right != nullptr )
        {
            delete right;
        }
        
        if ( left != nullptr )
        {
            delete left;
        }
    }
};
//---------------------------------------------------------------------------------------------------------
class HuffmanAlg
{
    std::vector< std::pair< std::string , float > >* charset_ptr;
    std::vector< std::pair< std::string , std::string > > codes;

    // Вектор указателей на вершину дерева, которая хранит данные вида: std::pair< буква , вероятность >
    std::vector< TreeTop< std::pair< std::string , float > >* > tree_tops;

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
    HuffmanAlg( std::vector< std::pair< std::string , float > >* charset_ptr_ )
    {
        entropy = 0;
        average_length = 0;
        redundancy = 0;

        charset_ptr = charset_ptr_;
        
        // Резервирование памяти под вершины дерева
        tree_tops.reserve( charset_ptr_->size() );
        
        for ( size_t i = 0 ; i < charset_ptr_->size() ; ++i )
        {
            tree_tops.push_back( new TreeTop< std::pair< std::string , float > >( ( *charset_ptr_ )[ i ] ));
        }

        // Резервирование памяти под коды
        codes.reserve( charset_ptr_->size() );
    }

    // Функция получения закодированного сообщения
    std::string archive( std::string& text )
    {
        std::string result = "";

        // Документация: https://en.cppreference.com/w/cpp/utility/bitset
        std::bitset< 4 > binSIZE_BLOCK( COMB_LENGTH );    // Перевод длины комбинации в битовое представление
        std::bitset< 16 > binBLOCKS_SIZE( codes.size() ); // Перевод количества ансамблей в битовое представление
        
        result += binSIZE_BLOCK.to_string() + binBLOCKS_SIZE.to_string();

        // Получение последовательности комбинаций и дерева в двоичном представлении
        std::string combs = "";
        std::string tree = "";

        tree_tops[ 0 ]->preorder_output( combs , tree );

        for ( auto iter = combs.begin() ; iter != combs.end() ; ++iter )
        {
            result += std::bitset<8>( *iter ).to_string();
        }

        result += tree + '0';

        // Кодирование текста
        for ( size_t i = 0 ; i < text.size() ; i += COMB_LENGTH )
        {
            std::string block = text.substr( i , COMB_LENGTH );
            result += codes[ find_character( codes , block ) ].second;
        }

        std::bitset<32> binSIZE( result.size() );       // Перевод размера закодированного текста в битовое представление 

        result = binSIZE.to_string() + result;

        if (result.size() % 8 != 0)
        {
            for (auto i = result.size() % 8; i < 8; i++)
            {
                result += "0";
            }
        }
        
        return result;
    }

    std::string extract( std::string& bin )
    {
        std::string result = "";

        const size_t SIZE = std::bitset< 32 >( bin.substr( 0 , 32 ) ).to_ullong();
        const size_t SIZE_BLOCK = std::bitset< 4 >( bin.substr( 32 , 4 ) ).to_ullong();
        const size_t BLOCKS_SIZE = std::bitset< 16 >( bin.substr( 36 , 16 ) ).to_ullong();

        charset_ptr->pop_back();
        charset_ptr->reserve( BLOCKS_SIZE );

        for ( size_t i = 0 ; i < BLOCKS_SIZE ; ++i )
        {
            std::string block = "";

            for ( size_t j = 0 ; j < SIZE_BLOCK ; ++j )
            {
                block += ( char )( std::bitset< 8 >( bin.substr( 52 + ( j * 8 ) + ( i * SIZE_BLOCK * 8 ) , 8 ) ).to_ulong() );
            }

            charset_ptr->push_back( { block , 0 } );
        }

        TreeTop< std::pair< std::string , float > >* current_top = tree_tops[ 0 ];

        size_t i = 0ull;
        int counter = 0;

        for ( size_t j = 0ull ; counter >= 0 ; ++i )
        {
            bool action = ( bool )( std::bitset< 1 >( bin[ 52 + BLOCKS_SIZE * SIZE_BLOCK * 8 + i ] ).to_ulong() );

            if ( action )
            {
                ++counter;

                TreeTop< std::pair< std::string , float > >* top = new TreeTop< std::pair< std::string , float > >( { "" , 0 } );

                if ( current_top->left == nullptr )
                {
                    current_top->push_left( top );
                }
                else
                {
                    current_top->push_right( top );
                }

                top->previous = current_top;
                current_top = top;
            }
            else
            {
                --counter;

                if ( current_top->left == nullptr && current_top->right == nullptr )
                {
                    current_top->data = ( *charset_ptr )[ j++ ];
                }

                current_top = current_top->previous;
            }
        }

        get_codes( tree_tops[ 0 ] , "" );

        std::string code = "";

        for ( size_t j = 52 + BLOCKS_SIZE * SIZE_BLOCK * 8 + i ; j < ( SIZE + 32 ) ; ++j )
        {
            code += bin[ j ];

            const int index = find_code( codes , code );

            if ( index >= 0 )
            {
                result += codes[ index ].first;

                code = "";
            }
        }

        return result;
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
    }

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
        for ( auto iter = codes.cbegin() ; iter != ( codes.cbegin() + 2 ) ; ++iter )
        {
            std::cout << iter->first << ": " << iter->second << std::endl;
        }

        return;
    }

    void archive(  )
    {
        
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

// Поиск буквы/комбинации в charset или codes
template < typename T >
int find_character( const std::vector< std::pair< std::string , T > >& charset , const std::string& character )
{
    for ( size_t i = 0 ; i < charset.size() ; ++i )
    {
        // Если буква/комбинация равна искомой
        if ( charset[i].first == character )
        {
            return (int)i;
        }
    }

    return -1;
}

// Поиск кода в codes
int find_code( const std::vector< std::pair< std::string , std::string > >& codes , const std::string& code )
{
    for ( size_t i = 0 ; i < codes.size() ; ++i )
    {
        // Если код равен искомому
        if ( codes[i].second == code )
        {
            return (int)i;
        }
    }

    return -1;
}

// Сортировка букв в codes по расположению букв/комбинаций в charset
void sort_codes_by_charset( const std::vector< std::pair< std::string , float > >* charset_ptr, // Указатель на charset
                            std::vector< std::pair< std::string , std::string > >& codes )      // Ссылка на codes
{
    std::pair< std::string , std::string > temp;
    int index;

    for ( size_t i = 0 ; i < charset_ptr->size() ; ++i )
    {
        // Поиск буквы/комбинации в codes ( < Тип шаблона >, соответствено float для charset и std::string для codes  )
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

// Генерация ансамбля всевозможных комбинаций и их вероятностей из ансамбля букв с их вероятностями
void generate_combinations( const std::vector< std::pair< std::string , float > >& charset,
                     std::vector< std::pair< std::string , float > >& combset )
{
    size_t size = charset.size();  // Количество символов
    std::vector< size_t > indices( COMB_LENGTH , 0 );  // Индексы для генерации комбинаций

    do
    {
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
        for ( int i = ( ( int )COMB_LENGTH - 1 ) ; i >= 0 ; --i )
        {
            if ( ++indices[i] < size ) // Увеличиваем текущий индекс
            {
                return true;
            }

            indices[i] = 0;  // Сбрасываем индекс, если достигнут предел
        }

        return false;  // Все комбинации сгенерированы
    }());
}

// Подсчет количества встреч подстроки substr в строке str
size_t count( const std::string& str , const std::string& substr )
{
    const size_t SUBSTR_SIZE = substr.size();
    const size_t STR_SIZE = str.size();
    
    size_t res = 0;
 
    for ( size_t i = 0 ; i <= ( STR_SIZE - SUBSTR_SIZE ) ; i += COMB_LENGTH )
    {
        size_t j = 0;

        for ( ; j < SUBSTR_SIZE ; ++j )
        {
            if ( str[ i + j ] != substr[ j ] )
            {
                break;
            }
        }
            
        if ( j == SUBSTR_SIZE )
        {
            ++res;
        }
    }

    return res;
}

void get_combinations( std::string& text , std::vector< std::pair< std::string , float > >& combset )
{
    // Добавляем к тексту пробелы, чтобы его длина была кратна длине комбинации
    while ( text.size() % COMB_LENGTH != 0 )
    {
        text += ' ';
    }

    const size_t TEXT_SIZE = text.size();
    const size_t COMB_AMOUNT = TEXT_SIZE / COMB_LENGTH; // Количество комбинаций в тексте

    // Резервирование памяти под максимально возможное количество уникальных комбинаций длины COMB_LENGTH
    combset.reserve( COMB_AMOUNT + 1 );

    float prob;
    std::pair< std::string , float > element;

    float sum = 0.0F;

    // Нахождение комбинаций и их частот, их добавление в ансамбль
    for ( size_t i = 0 ; i < TEXT_SIZE ; i += COMB_LENGTH )
    {
        const std::string combination = text.substr( i , COMB_LENGTH );
        size_t comb_count = count( text , combination );

        prob = ( float )comb_count / ( float )( COMB_AMOUNT ); // Подсчет вероятности
        element = { combination , prob };

        // Если ансамбле combset нету найденной комбинации, то добавляем ее в конец ансамбля
        if ( ( std::find( combset.begin() , combset.end() , element ) ) == combset.end() )
        {
            combset.push_back( element );
            sum += prob;
        }
    }

    return;
}

void writeBinaryToFile( std::string binaryString , const std::string& fileName )
{
    std::ofstream outFile( fileName , std::ios::binary );

    size_t len = binaryString.length();

    for ( size_t i = 0 ; i < len ; i += 8ull )
    {
        std::bitset< 8 > bits( binaryString.substr( i , 8 ) );
        unsigned char byte = static_cast< unsigned char >( bits.to_ulong() );
        outFile.write( reinterpret_cast< const char* >( &byte ) , sizeof( byte ) );
    }

    outFile.close();
}

std::string readBinaryStringFromFile( const std::string &fileName )
{
    std::ifstream inFile( fileName , std::ios::binary );
    std::string stringBin;

    unsigned char byte;

    while ( inFile.read( reinterpret_cast< char* >( &byte ) , sizeof( byte ) ) )
    {
        std::bitset< 8 > bits( byte );
        stringBin += bits.to_string();
    }

    inFile.close();

    return stringBin;
}

//---------------------------------------------------------------------------------------------------------
int main(int, char**)
{
    // std::string text = "abcdef";

    // std::string substr = text.substr( 8 , 4 );

    std::string file_path = "../../../text.txt";
    
    std::cout << "Input path to file:" << std::endl;
    std::cout << file_path << std::endl;

    bool status;

    std::cout << "What you want to do?\n0 - Archive\n1 - Extract" << std::endl;
    std::cin >> status;

    if ( !status )
    {
        std::cout << "What algorithm you want to use?\n0 - Huffman algorithm\n1 - Shennon-Fano algorithm" << std::endl;

        std::cin >> status;

        if ( !status )
        {
            std::string text;

            std::ifstream file;

            file.open( file_path , std::ios::binary );

            if ( file.is_open() )
            {
                std::string line;

                char delimiter = 0x3;

                if ( std::getline( file , line , delimiter ) )
                {
                    text = line;
                }
            }

            file.close();

            std::vector< std::pair< std::string , float > > combset;

            get_combinations( text , combset );

            HuffmanAlg huffman_alg( &combset );

            huffman_alg.output_codes();

            std::string archive = huffman_alg.archive( text );

            writeBinaryToFile( archive , "../../../text_archived.bin" );
        }
    }
    else
    {
        std::string archive = readBinaryStringFromFile( "../../../text_archived.bin" );

        std::vector< std::pair< std::string , float > > combset{ { "" , 0 } };

        HuffmanAlg huffman_alg( &combset );

        std::string text = huffman_alg.extract( archive );

        std::ofstream file;

        file.open( "../../../text_new.txt" , std::ios::binary );

        if ( file.is_open() )
        {
            file << text;
        }

        file.close();
    }

    return 0;
}