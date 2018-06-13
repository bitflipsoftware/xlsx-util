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

    // std::string numtolet( int num );
    Row extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    Table extractAllRows( const char* sheetname, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders );


    inline Table
    extractAllData( const std::string& filename )
    {
        std::cout << "begin getDataAsync" << std::endl;
        iq::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            return Table{};
        }

        const char* sheetname = nullptr;
        std::map<int, std::string> headers;
        std::cout << "about to call extractAllRows from getDataAsync" << std::endl;
        Table tbl = extractAllRows( sheetname, xreader, headers );
        std::cout << "executing return returnArr from getDataAsync" << std::endl;
        return tbl;
    }


    inline Table
    extractAllRows( const char* sheetname, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders )
    {
        std::cout << "begin extractAllRows" << std::endl;
        Table tbl;
        xlsxioreadersheet sheet = nullptr;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                auto row = extractRow( sheet, ioHeaders );
                tbl.emplace_back( std::move( row ) );
            }

            xlsxioread_sheet_close(sheet);
        }

        return tbl;
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


    // std::string numtolet( int num )
    // {
    //     std::cout << "begin numtolet" << std::endl;
    //     std::stringstream ss;
    //     std::string result;
    //     int i = 0;
        
    //     while( num > 0 )
    //     {
    //         int rem = num % 26;
            
    //         // If remainder is 0, then a 'Z' must be there in output
    //         if( rem == 0 )
    //         {
    //             ss << std::string{ 'Z' };
    //             ++i;
    //             num = ( num / 26 ) - 1;
    //         }
    //         else // If remainder is non-zero
    //         {
    //             const char a = 'A';
    //             const char remC = static_cast<char>( rem );
    //             const char remCm = remC - static_cast<char>( 1 );
    //             const char theC = a + remCm;
    //             ss << std::string{ theC };
    //             ++i;
    //             num = num / 26;
    //         }
    //     }
        
    //     result = ss.str();
        
    //     // Reverse the string and print result
    //     std::reverse( std::begin(result), std::end(result) );
    //     return result;
    // }
}

