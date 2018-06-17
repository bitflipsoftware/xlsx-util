#pragma once
#include <string>

namespace xlsx
{
    enum class ValType
    {
        None,
        Double,
        Int,
        String
    };

    class Val
    {
    public:
        Val();
        explicit Val( std::string val );
        explicit Val( double val );
        explicit Val( int val );

    public:
        bool getIsString() const;
        bool getIsDouble() const;
        bool getIsInt() const;
        const std::string& getString() const;
        double getDouble() const;
        int getInt() const;
        void setString( std::string val );
        void setDouble( double val );
        void setInt( int val );

        // will set as number if it can be parsed to a number
        void setParse( const std::string& val );

    private:
        ValType myType;
        std::string myString;
        double myDouble;
        int myInt;

    private:
        static bool isScientific( const std::string& inVal, double& outVal );
        static bool isDecimal( const std::string& inVal, double& outVal );
        static bool isInteger( const std::string& inVal, int& outVal );
        static bool isString( const std::string& inVal );
    };
}