#include <iostream>

int main(int, char**){
    const size_t n = 8;

    float probs[n]{ 0.26 , 0.24 , 0.15 , 0.12 , 0.09 , 0.07 , 0.05 , 0.02 };

    for ( size_t i = 0 ; i < n ; ++i )
    {
        std::cout << "z" << i << ": " << probs[i] << " ";
    }

    std::cout << std::endl;

    

    return 0;
}