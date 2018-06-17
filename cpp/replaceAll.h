#pragma once
#include "toLower.h"
#include <string>

namespace xlsx
{
    inline std::string replaceAll( std::string theString, const std::string& findStr, const std::string& replaceWith )
    {
        const auto theStringLowercase = toLower( theString );
        const auto findStrLowercase = toLower( findStr );

        std::string::size_type pos = 0;
        const auto findStrLen = findStr.size();
        const auto replaceWithLen = replaceWith.size();

        while ( ( pos = theStringLowercase.find( findStrLowercase, pos ) ) != std::string::npos )
        {
            // cautionary: in case of UTF non-ascii issues
            if( pos >= theString.size() || pos + findStrLen > theString.size() )
            {
                break;
            }

            theString = theString.replace( pos, findStrLen, replaceWith );
            pos += replaceWithLen;
        }

        return theString;
    }
}
