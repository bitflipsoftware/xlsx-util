#pragma once
#include "xlsx.h"
#include "xlsxio_read.h"

#include <iostream>
#include <future>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace xlsx
{
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    std::string numtolet( int num );
    Row extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    Row extractHeaders( xlsxioreadersheet sheet );
    Table extractAllRows( const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders );


    inline std::string
    extractAllData( const std::string& filename )
    {
        std::cout << "begin getDataAsync" << std::endl;
        iq::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            return Table{};
        }

        Table returnArr;
        const char* sheetname = nullptr;
        std::map<int, std::string> headers;
        std::cout << "about to call extractAllRows from getDataAsync" << std::endl;
        returnArr[static_cast<uint32_t>( 0 )] = extractAllRows( scope, sheetname, hasHeaders, xreader, headers );
        std::cout << "executing return returnArr from getDataAsync" << std::endl;
        return returnArr;
    }


    inline Table
    extractAllRows( const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders )
    {
        std::cout << "begin extractAllRows" << std::endl;
        auto returnArr = XXXXXXArray::New( scope.Env()  );
        xlsxioreadersheet sheet = nullptr;
        int rowIndex = 0;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            bool headersParsed = false;
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                const auto row = extractRow( scope, sheet, ioHeaders );
                returnArr[static_cast<uint32_t>( rowIndex )] = row;
                ++rowIndex;
            }

            xlsxioread_sheet_close(sheet);
        }

        return returnArr;
    }


    inline Row
    extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders )
    {
        std::cout << "begin extractRow" << std::endl;
        char* valueCstr = nullptr;
        Row row{};
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            
            const std::string val{ valueCstr };
            row.emplace_back( std::string{ valueCstr } );
            free( valueCstr );
            valueCstr = nullptr;
        }

        return row;
    }


    inline std::vector<std::string>
    extractHeaders( xlsxioreadersheet sheet )
    {
        std::cout << "begin extractHeaders" << std::endl;
        char* valueCstr = nullptr;
        std::vector<std::string> headers;
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            const std::string val{ valueCstr };
//            XXXXXXValue napiString = extractValue( scope, val );
            free( valueCstr );
            valueCstr = nullptr;
            headers.push_back( val );
        }

        return headers;
    }


    std::string numtolet( int num )
    {
        std::cout << "begin numtolet" << std::endl;
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
        
        // Reverse the string and print result
        std::reverse( std::begin(result), std::end(result) );
        return result;
    }
}

