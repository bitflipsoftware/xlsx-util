#include "napi.h"
#include "node_api.h"
#include "toss.h"
#include "str.h"
#include "xlsx.h"
#include "xlsxio_read.h"

#include <iostream>
#include <future>
#include <algorithm>
#include <sstream>
#include <string>
#include <map>
#include <vector>

namespace iq
{
    std::string numtolet( int num );
    Napi::Value extractValue( Napi::Env& env, char*& cstr );
    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    std::vector<std::string> extractHeaders( Napi::Env& env, xlsxioreadersheet sheet );
    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders );
    std::string findHeaderName( int columnIndex, std::map<int, std::string>& ioHeaders );

    struct XlsxOptions
    {
        std::map<std::string, std::string> headerOverrides;
    };

    Napi::Array getDataAsync( Napi::Env& env, const std::string& filename, bool hasHeaders, const XlsxOptions& options )
    {
        iq::XlsxReader xreader{ filename };

        if( !xreader.getIsOk() )
        {
            return Napi::Array::New( env );
        }

        const auto sheetNames = xreader.getSheetNames();

        if( sheetNames.empty() )
        {
            return Napi::Array::New( env );
        }

        auto returnArr = Napi::Array::New( env );
        const char* sheetname = nullptr;
        std::map<int, std::string> headers;
        returnArr[static_cast<uint32_t>( 0 )] = extractAllRows( env, sheetname, hasHeaders, xreader, headers );
        return returnArr;
    }


    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders )
    {
        auto returnArr = Napi::Array::New( env );
        xlsxioreadersheet sheet = nullptr;
        int rowIndex = 0;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            bool headersParsed = false;
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                if ( hasHeaders && rowIndex == 0 && !headersParsed )
                {
                    const auto foundHeaders = extractHeaders( env, sheet );
                    
                    for( size_t i = 0; i < foundHeaders.size(); ++i )
                    {
                        auto foundHeader = foundHeaders.at( i );
                        
                        if( foundHeader.empty() )
                        {
                            foundHeader = numtolet( i + 1 );
                        }

                        ioHeaders[i] = foundHeaders.at( i );
                    }

                    headersParsed = true;
                }
                else
                {
                    const auto row = extractRow( env, sheet, ioHeaders );
                    returnArr[static_cast<uint32_t>( rowIndex )] = row;
                    ++rowIndex;
                }
            }

            xlsxioread_sheet_close(sheet);
        }

        return returnArr;
    }


    std::string findHeaderName( int columnIndex, std::map<int, std::string>& ioHeaders )
    {
        const auto iter = ioHeaders.find( columnIndex );
        
        if( iter == ioHeaders.cend() )
        {
            const auto name = numtolet( columnIndex + 1 );
            ioHeaders[columnIndex] = name;
            return name;
        }

        return iter->second;
    }


    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders )
    {
        char* valueCstr = nullptr;
        int cellIndex = 0;
        auto row = Napi::Object::New( env );
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            auto napiString = extractValue( env, valueCstr );
            const auto cellIndexNapi = Napi::String::New( env, findHeaderName( cellIndex, ioHeaders ) );
            row.Set(cellIndexNapi, napiString);
            ++cellIndex;
        }

        return row;
    }


    std::vector<std::string> extractHeaders( Napi::Env& env, xlsxioreadersheet sheet )
    {
        char* valueCstr = nullptr;
        std::vector<std::string> headers;
        
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            auto napiString = extractValue( env, valueCstr );
            const auto stdstring = napiString.As<Napi::String>().Utf8Value();
            headers.push_back( stdstring );
        }

        return headers;
    }


    Napi::Value extractValue( Napi::Env& env, char*& cstr )
    {
        // TODO - convert numbers to numbers
        // TODO - convert scientific notation to number
        const std::string val{ cstr };
        free( cstr );
        cstr = nullptr;
        auto napiString = Napi::String::New( env, val );
        return napiString;
    }


    Napi::Promise getData( const Napi::CallbackInfo& info )
    {
        Napi::Env env = info.Env();

        auto deferred = Napi::Promise::Deferred::New(env);

        if ( info.Length() != 3 )
        {
            deferred.Reject( Napi::TypeError::New( env, "three arguments are required: string filename, bool isFirstRowHeaders, object options").Value() );
            return deferred.Promise();
        }
        else if( !info[0].IsString()  )
        {
            deferred.Reject( Napi::TypeError::New( env, "the first argument must be a string: filename").Value() );
            return deferred.Promise();
        }
        else if( !info[1].IsBoolean()  )
        {
            deferred.Reject( Napi::TypeError::New( env, "the second argument must be a boolean: isFirstRowHeaders").Value() );
            return deferred.Promise();
        }
        else if( !info[2].IsObject()  )
        {
            deferred.Reject( Napi::TypeError::New( env, "the third argument must be an object containing options: options").Value() );
            return deferred.Promise();
        }
        else if( !info[2].ToObject().Has( "headerOverrides" ) || !info[2].ToObject().Get( "headerOverrides" ).IsObject() )
        {
            deferred.Reject( Napi::TypeError::New( env, "the options object must contain a headerOverrides object").Value() );
            return deferred.Promise();
        }

        const auto headerOverridesObj = info[2].ToObject().Get( "headerOverrides" ).ToObject();

        napi_value napi_property_names = nullptr;
        const auto status = napi_get_property_names( static_cast<napi_env>( env ), static_cast<napi_value>( headerOverridesObj ), &napi_property_names );

        if( status != napi_ok )
        {
            deferred.Reject( Napi::Error::New( env, "error while reading the headerOverrides object").Value() );
            return deferred.Promise();
        }

        Napi::Value propNamesValue( static_cast<napi_env>( env ), napi_property_names );

        if( !propNamesValue.IsArray() )
        {
            deferred.Reject( Napi::Error::New( env, "proNamesValue should have been an array").Value() );
            return deferred.Promise();
        }

        const Napi::Array propNamesArray = headerOverridesObj.GetPropertyNames();

        const auto filename = info[0].ToString().Utf8Value();

        std::future<Napi::Array> fut = std::async( std::launch::async, getDataAsync, std::ref( env ), filename, info[1].ToBoolean(), XlsxOptions{} );

        auto returnArr = fut.get();

        if( returnArr.Length() == 0 )
        {
            deferred.Reject( Napi::TypeError::New( env, "xlsx file could not be opened" ).Value() );
            return deferred.Promise();
        }

        deferred.Resolve( returnArr );
        return deferred.Promise();
    }


    std::string numtolet( int num )
    {
        std::stringstream ss;
        std::string result;
        int i = 0; // To store current index in str which is result
        
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

