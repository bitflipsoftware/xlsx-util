#pragma once
#include <string>
#include <locale>
#include <sstream>

namespace xlsx
{
    inline std::string normalizeString( const std::string& theString )
    {
        std::locale loc;
        std::stringstream result;
        
        for( std::string::size_type i = 0; i < theString.length(); ++i )
        {
            if( std::isalnum( static_cast<unsigned char>( theString[i] ) ) )
            {
                result << std::tolower( theString[i], loc );
            }
        }
        
        return result.str();
    }
}
