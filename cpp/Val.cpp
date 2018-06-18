#include "Val.h"
#include <regex>
#include <iostream>
#include <cmath>
#include <sstream>

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
        if( val.empty() )
        {
            myType = ValType::None;
            myString = std::string{};
            myDouble = 0.0;
            myInt = 0.0;
            return;
        }

        int intVal = 0;
        if( Val::isInteger( val, intVal ) )
        {
            setInt( intVal );
            return;
        }

        double doubleVal = 0.0;

        if( Val::isScientificOrDecimal( val, doubleVal ) )
        {
            setDouble( doubleVal );
            return;
        }

        setString( val );
    }


    enum class SciPart
    {
        start,
        baseSign,
        baseInt,
        basePoint,
        baseDec,
        e,
        expSign,
        expInt,
    };


    bool
    Val::isScientificOrDecimal( const std::string& inVal, double& outVal )
    {
        outVal = 0.0;
        SciPart part = SciPart::start;
        std::stringstream base;
        std::stringstream exp;

        for( const auto c : inVal )
        {
            if( part == SciPart::start )
            {
                if( c == '-' )
                {
                    base << c;
                    part = SciPart::baseSign;
                }
                else if( std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    part = SciPart::baseInt;
                    base << c;
                }
                else
                {
                    return false;
                }
            }
            else if( part == SciPart::baseSign )
            {
                if( !std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    return false;
                }
                else
                {
                    part = SciPart::baseInt;
                    base << c;
                }
            }
            else if( part == SciPart::baseInt )
            {
                if( c == '.' )
                {
                    base << c;
                    part = SciPart::basePoint;
                }
                else if( std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    base << c;
                }
                else
                {
                    return false;
                }
            }
            else if( part == SciPart::basePoint )
            {
                if( !std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    return false;
                }
                else
                {
                    part = SciPart::baseDec;
                    base << c;
                }
            }
            else if( part == SciPart::baseDec )
            {
                if( c == 'E' || c == 'e' )
                {
                    part = SciPart::e;
                }
                else if( std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    base << c;
                }
                else
                {
                    return false;
                }
            }
            else if( part == SciPart::e )
            {
                if( c == '-' )
                {
                    exp << c;
                    part = SciPart::expSign;
                }
                else if( std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    part = SciPart::expInt;
                    exp << c;
                }
                else
                {
                    return false;
                }
            }
            else if( part == SciPart::expSign )
            {
                if( !std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    return false;
                }
                else
                {
                    part = SciPart::expInt;
                    exp << c;
                }
            }
            else if( part == SciPart::expInt )
            {
                if( !std::isdigit( static_cast<unsigned char>( c ) ) )
                {
                    return false;
                }
                else
                {
                    exp << c;
                }
            }
            else
            {
                return false;
            }
        }

        const std::string b = base.str();
        const std::string e = exp.str();
        
        if( b.empty() )
        {
            return false;
        }

        if( e.empty() )
        {
            // optimization, this is probably a decimal number so we will handle it
            const double shortcutDouble = std::stod( b );
            outVal = shortcutDouble;
            return true;
        }
        
        const double baseDouble = std::stod( b );
        const double expDouble = std::stod( e );
        const double multDouble = std::pow( 10.0, expDouble );
        const double valueDouble = baseDouble * multDouble;
        outVal = valueDouble;
        return true;
    }


    bool
    Val::isInteger( const std::string& inVal, int& outVal )
    {
        outVal = 0;
        bool isFirstChar = true;

        for( const auto& c : inVal )
        {
            if( isFirstChar && c == '-' )
            {
                isFirstChar = false;
                continue;
            }
            else if( !std::isdigit( static_cast<unsigned char>( c ) ) )
            {
                return false;
            }

            isFirstChar = false;
        }

        outVal = std::stoi( inVal );
        return true;
    }
}
