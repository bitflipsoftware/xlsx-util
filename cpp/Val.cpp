#include "Val.h"
#include <regex>
#include <iostream>
#include <cmath>

namespace xlsx
{
    Val::Val()
    : myType{ ValType::None }
    , myString{}
    , myDouble{ 0 }
    , myInt{ 0 }
    {

    }


    Val::Val( std::string val )
    : Val{}
    {
        setString( std::move( val ) );
    }


    Val::Val( double val )
    : Val{}
    {
        setDouble( val );
    }


    Val::Val( int val )
    : Val{}
    {
        setInt( val );
    }


    bool
    Val::getIsString() const
    {
        return myType == ValType::String;
    }


    bool
    Val::getIsDouble() const
    {
        return myType == ValType::Double;
    }


    bool
    Val::getIsInt() const
    {
        return myType == ValType::Int;
    }


    const std::string&
    Val::getString() const
    {
        return myString;
    }


    double
    Val::getDouble() const
    {
        return myDouble;
    }


    int
    Val::getInt() const
    {
        return myInt;
    }


    void
    Val::setString( std::string val )
    {
        myType = ValType::String;
        myString = std::move( val );
        myDouble = 0;
        myInt = 0;
    }


    void
    Val::setDouble( double val )
    {
        myType = ValType::Double;
        myString = "";
        myDouble = val;
        myInt = 0;
    }


    void
    Val::setInt( int val )
    {
        myType = ValType::Int;
        myString = "";
        myDouble = 0;
        myInt = val;
    }


    void
    Val::setParse( const std::string& val )
    {
        const std::string patternSci = R"(([0-9]+\.[0-9]+)[eE](-?[0-9]+))"; 
        std::regex rxSci{ patternSci };
        std::smatch matchSci;
        const bool isSci = std::regex_search(val, matchSci, rxSci);

        if( isSci )
        {
            if( matchSci.size() == 3 )
            {
                const std::string b = matchSci[1];
                const std::string e = matchSci[2];
                const double base = std::stod( b );
                const double exp = std::stod( e );
                const double mult = std::pow( 10.0, exp );
                const double value = base * mult;
                setDouble( value );
                return;
            }
        }

        bool isStr = false;
        bool isFirstChar = true;
        int decCount = 0;

        for( const auto& c : val )
        {
            if( isFirstChar && c == '-' )
            {
                isFirstChar = false;
                continue;
            }
            else if( std::isdigit( static_cast<unsigned char>( c ) ) )
            {
                isFirstChar = false;
                continue;
            }
            else if( c == '.' && decCount == 0 )
            {
                ++decCount;
                isFirstChar = false;
                continue;
            }
            else
            {
                isStr = true;
                break;
            }

            isFirstChar = false;
        }

        if( isStr )
        {
            setString( val );
            return;
        }

        const std::string patternDec = R"(^[-]?[0-9]+\.[0-9]+$)"; 
        std::regex rxDec{ patternDec };
        std::smatch matchDec;
        const bool isDec = std::regex_search(val, matchDec, rxDec);

        if( isDec )
        {
            if( matchDec.size() == 1 )
            {
                const std::string d = matchDec[0];
                const double num = std::stod( d );
                setDouble( num );
                return;
            }
        }

        const std::string patternInt = R"([-]?[0-9]+)"; 
        std::regex rxInt{ patternInt };
        std::smatch matchInt;
        const bool isInt = std::regex_search(val, matchInt, rxInt);

        if( isInt )
        {
            if( matchInt.size() == 1 )
            {
                const std::string i = matchInt[0];
                const int num = std::stoi( i );
                setInt( num );
                return;
            }
        }

        setString( val );
    }
}