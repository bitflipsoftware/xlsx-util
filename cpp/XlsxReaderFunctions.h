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

    Row extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    Table extractAllRows( const char* sheetname, xlsx::XlsxReader& xreader, std::map<int, std::string>& ioHeaders );

    inline Table
    extractAllData( const std::string& filename )
    {
        xlsx::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            std::stringstream ss;
            ss << "xlsx-util: the file '" << filename << "' could not be opened" << std::endl;
            throw std::runtime_error{ ss.str() };
        }

        const char* sheetname = nullptr;
        std::map<int, std::string> headers;
        Table tbl = extractAllRows( sheetname, xreader, headers );
        return tbl;
    }


    inline Table
    extractAllRows( const char* sheetname, xlsx::XlsxReader& xreader, std::map<int, std::string>& ioHeaders )
    {
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
}

