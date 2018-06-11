#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

inline std::string numtolet( int num )
{
    std::stringstream ss;
    std::string result;
    int i = 0; // To store current index in str which is result
    
    while( num > 0 )
    {
        int rem = num % 26;
        
        // If remainder is 0, then a 'Z' must be there in output
        if( rem == 0 )
        {
            ss << std::string{ 'Z' };
            ++i;
            num = ( num / 26 ) - 1;
        }
        else // If remainder is non-zero
        {
            const char a = 'A';
            const char remC = static_cast<char>( rem );
            const char remCm = remC - static_cast<char>( 1 );
            const char theC = a + remCm;
            ss << std::string{ theC };
            ++i;
            num = num / 26;
        }
    }
    
    result = ss.str();
    
    // Reverse the string and print result
    std::reverse( std::begin(result), std::end(result) );
    return result;
}

int main(int argc, const char * argv[]) {
    std::cout << numtolet( 705 ) << std::endl;
    std::cout << numtolet( 1 ) << std::endl;
    std::cout << numtolet( 0 ) << std::endl;
    return 0;
}
