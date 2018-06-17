#pragma once
#include <string>
#include <sstream>
#include <algorithm>

namespace xlsx
{
    inline std::string 
    numtolet( int num )
    {
        std::stringstream ss;
        std::string result;
        int i = 0;
        
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
        std::reverse( std::begin(result), std::end(result) );
        return result;
    }
}