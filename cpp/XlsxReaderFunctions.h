#pragma once
#include "xlsx.h"
#include "xlsxio_read.h"
#include "Sheet.h"
#include "Val.h"

#include <iostream>
#include <future>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace xlsx
{
    using Row = std::vector<Val>;
    using Table = std::vector<Row>;

    Row extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    Sheet extractAllRows( const char* sheetname, xlsx::XlsxReader& xreader, std::map<int, std::string>& ioHeaders );

    inline Sheet
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
        Sheet sh = extractAllRows( sheetname, xreader, headers );
        return sh;
    }


    inline Sheet
    extractAllRows( const char* sheetname, xlsx::XlsxReader& xreader, std::map<int, std::string>& ioHeaders )
    {
        Sheet result;
        xlsxioreadersheet sheet = nullptr;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                auto row = extractRow( sheet, ioHeaders );
                result.addRow( std::move( row ) );
            }

            xlsxioread_sheet_close(sheet);
        }

        return result;
    }


    inline Row
    extractRow( xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders )
    {
        char* valueCstr = nullptr;
        Row row{};
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            const std::string str{ valueCstr };
            Val v;
            v.setParse( str );
            row.emplace_back( std::move( v ) );
            free( valueCstr );
            valueCstr = nullptr;
        }

        return row;
    }
}

