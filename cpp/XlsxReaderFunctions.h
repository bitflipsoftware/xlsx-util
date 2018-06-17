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
#include <locale>
#include <cctype>

namespace xlsx
{
    Row extractRow( xlsxioreadersheet sheet, int& ioRowSize );
    
    Sheet extractAllRows(
        const char* sheetname,
        xlsx::XlsxReader& xreader,
        bool hasHeaders,
        const std::map<std::string, std::string>& headerTransformMap,
        const std::set<std::string>& deletes,
        bool doPascalCase,
        const std::set<std::string>& pascalWords );

    inline Sheet
    extractAllData(
        const std::string& filename,
        bool hasHeaders,
        const std::map<std::string, std::string>& headerTransformMap,
        const std::set<std::string>& deletes,
        bool doPascalCase,
        const std::set<std::string>& pascalWords )
    {
        xlsx::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            std::stringstream ss;
            ss << "xlsx-util: the file '" << filename << "' could not be opened" << std::endl;
            throw std::runtime_error{ ss.str() };
        }

        const char* sheetname = nullptr;
        Sheet sh = extractAllRows( sheetname, xreader, hasHeaders, headerTransformMap, deletes, doPascalCase, pascalWords );
        return sh;
    }


    inline Sheet
    extractAllRows(
        const char* sheetname,
        xlsx::XlsxReader& xreader,
        bool hasHeaders,
        const std::map<std::string, std::string>& headerTransformMap,
        const std::set<std::string>& deletes,
        bool doPascalCase,
        const std::set<std::string>& pascalWords )
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
            Row* row = result.getMutableRow( i );
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

        // check for delete strings before altering the headers
        std::set<int> deleteIndices;

        if( !deletes.empty() )
        {
            int headerIndex = 0;
            for( const auto& header : headers )
            {
                const auto it = deletes.find( header );

                if( it != deletes.cend() )
                {
                    deleteIndices.insert( headerIndex );
                }

                ++headerIndex;
            }
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

        // check for delete strings again after altering the headers
        if( !deletes.empty() )
        {
            int headerIndex = 0;
            for( const auto& header : headers )
            {
                const auto it = deletes.find( header );

                if( it != deletes.cend() )
                {
                    deleteIndices.insert( headerIndex );
                }

                ++headerIndex;
            }
        }

        if( doPascalCase )
        {
            std::vector<std::string> newHeaders;
            int hindex = 0;

            for( const auto& header : headers )
            {
                std::stringstream ss;
                bool wasSpace = true;
                bool isFirstChar = true;

                for( const auto c : header )
                {
                    if( !std::isalnum( c ) )
                    {
                        wasSpace = true;
                        continue;
                    }

                    std::string current;

                    if( isFirstChar && std::isdigit( c ) )
                    {
                        switch(c)
                        {
                            case '0': { current = "Zero"; break; }
                            case '1': { current = "One"; break; }
                            case '2': { current = "Two"; break; }
                            case '3': { current = "Three"; break; }
                            case '4': { current = "Four"; break; }
                            case '5': { current = "Five"; break; }
                            case '6': { current = "Six"; break; }
                            case '7': { current = "Seven"; break; }
                            case '8': { current = "Eight"; break; }
                            case '9': { current = "Nine"; break; }
                            default: { throw std::runtime_error{ "this should never happen" }; }
                        }
                    }
                    else if( std::isdigit( c ) )
                    {
                        current = std::string{ c };
                    }
                    else if( wasSpace )
                    {
                        current = std::string{ static_cast<char>( std::toupper( static_cast<unsigned char>( c ) ) ) };
                    }
                    else
                    {
                        current = std::string{ static_cast<char>( std::tolower( static_cast<unsigned char>( c ) ) ) };
                    }

                    isFirstChar = false;
                    wasSpace = false;
                    ss << current;
                }

                auto newHeader = ss.str();

                if( !newHeader.empty() )
                {
                    for( const auto& replacement : pascalWords )
                    {
                        std::stringstream rss;

                        for( const auto r : replacement )
                        {
                            rss << std::tolower( static_cast<char>( static_cast<unsigned char>( r ) ) );
                        }

                        const std::string l = rss.str();

                        std::string::size_type n = 0;
                        while ( ( n = newHeader.find( l, n ) ) != std::string::npos )
                        {
                            newHeader.replace( n, l.size(), replacement );
                            n += replacement.size();
                        }
                    }

                    newHeaders.push_back( newHeader );
                }
                else
                {
                    newHeaders.push_back( numtolet( hindex + 1 ) );
                }

                ++hindex;
            }

            headers = newHeaders;
        }

        // check for delete strings again after altering the headers
        if( !deletes.empty() )
        {
            int headerIndex = 0;
            for( const auto& header : headers )
            {
                const auto it = deletes.find( header );

                if( it != deletes.cend() )
                {
                    deleteIndices.insert( headerIndex );
                }

                ++headerIndex;
            }
        }

        for( auto it = deleteIndices.crbegin(); it != deleteIndices.crend(); ++it )
        {
            for( int z = 0; z < numRows; ++z )
            {
                Row* row = result.getMutableRow( z );
                auto delIter = row->begin() + static_cast<size_t>( *it );
                row->erase( delIter );
            }

            auto delHeaderIter = headers.begin() + static_cast<size_t>( *it );
            headers.erase( delHeaderIter );
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

