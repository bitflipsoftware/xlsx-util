#pragma once
#include <string>
#include <locale>
#include <sstream>

namespace xlsx
{
    inline std::string toLower( std::string theString )
    {

        std::locale loc;
        std::stringstream result;
        
        for( std::string::size_type i = 0; i < theString.length(); ++i )
        {
            result << std::tolower( theString[i], loc );
        }
        
        return result.str();
    }
}
