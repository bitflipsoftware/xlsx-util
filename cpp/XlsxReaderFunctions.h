#pragma once
#include "xlsx.h"
#include "xlsxio_read.h"
#include "Sheet.h"
#include "Val.h"
#include "numtolet.h"

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

    Row extractRow( xlsxioreadersheet sheet, int& ioRowSize );
    
    Sheet extractAllRows(
        const char* sheetname,
        xlsx::XlsxReader& xreader,
        bool hasHeaders,
        const std::map<std::string, std::string>& headerTransformMap );

    inline Sheet
    extractAllData( const std::string& filename, bool hasHeaders, const std::map<std::string, std::string>& headerTransformMap )
    {
        xlsx::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            std::stringstream ss;
            ss << "xlsx-util: the file '" << filename << "' could not be opened" << std::endl;
            throw std::runtime_error{ ss.str() };
        }

        const char* sheetname = nullptr;
        Sheet sh = extractAllRows( sheetname, xreader, hasHeaders, headerTransformMap );
        return sh;
    }


    inline Sheet
    extractAllRows(
        const char* sheetname,
        xlsx::XlsxReader& xreader,
        bool hasHeaders,
        const std::map<std::string, std::string>& headerTransformMap )
    {
        Sheet result;
        xlsxioreadersheet sheet = nullptr;
        int maxRowSize = 0;
        bool isFirstRow = true;
        std::vector<std::string> headers;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                int rowSize = 0;
                auto row = extractRow( sheet, rowSize );
                maxRowSize = std::max( maxRowSize, rowSize );

                if( isFirstRow && hasHeaders )
                {
                    isFirstRow = false;
                    int headerIndex = 0;

                    for( const auto& val : row )
                    {
                        if( val.getIsString() )
                        {
                            headers.push_back( val.getString() );
                        }
                        else
                        {
                            headers.push_back( numtolet( headerIndex + 1 ) );
                        }

                        ++headerIndex;
                    }
                }
                else
                {
                    result.addRow( std::move( row ) );
                }
            }

            xlsxioread_sheet_close(sheet);
        }

        const int numRows = result.getNumRows();
        
        for( int i = 0; i < numRows; ++i )
        {
            std::vector<Val>* row = result.getMutableRow( i );
            while( static_cast<int>( row->size() ) < maxRowSize )
            {
                row = result.getMutableRow( i );
                row->emplace_back( Val{} );
            }
        }

        while( headers.size() < static_cast<size_t>( maxRowSize ) )
        {
            const int nextColumnIndex = static_cast<int>( headers.size() );
            auto nextColumnLetters = numtolet( nextColumnIndex + 1 );
            headers.emplace_back( std::move( nextColumnLetters ) );
        }

        if( !headerTransformMap.empty() )
        {
            for( auto it = headers.begin(); it != headers.end(); ++it )
            {
                const auto oldHeader = *it;
                const auto findIter = headerTransformMap.find( oldHeader );

                if( findIter != headerTransformMap.cend() )
                {
                    const auto newHeader = findIter->second;
                    *it = newHeader;
                }
            }
        }


        result.setHeaders( std::move( headers ) );
        return result;
    }


    inline Row
    extractRow( xlsxioreadersheet sheet, int& ioRowSize )
    {
        char* valueCstr = nullptr;
        Row row{};
        ioRowSize = 0;
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            const std::string str{ valueCstr };
            Val v;
            v.setParse( str );
            row.emplace_back( std::move( v ) );
            free( valueCstr );
            valueCstr = nullptr;
            ++ioRowSize;
        }

        return row;
    }
}

