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

    struct XlsxOptions
    {
        Napi::Function headerTransform;
    };

    std::string numtolet( int num );
    // int lettonum( const std::string& let );
    Napi::Value extractValue( Napi::Env& env, char*& cstr );
    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    std::vector<std::string> extractHeaders( Napi::Env& env, xlsxioreadersheet sheet );
    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders, const XlsxOptions& options );
    std::string findHeaderName( int columnIndex, std::map<int, std::string>& ioHeaders );


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
        returnArr[static_cast<uint32_t>( 0 )] = extractAllRows( env, sheetname, hasHeaders, xreader, headers, options );
        return returnArr;
    }


    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders, const XlsxOptions& options )
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
                    auto foundHeaders = extractHeaders( env, sheet );
                    
                    if( !options.headerTransform.IsNull() && !foundHeaders.empty() )
                    {
                        auto arr = Napi::Array::New( env );

                        for( int i = 0; i < foundHeaders.size(); ++i )
                        {
                            arr[i] = Napi::String::New( env, foundHeaders.at( i ) );
                            std::cout << foundHeaders.at( i ) << std::endl;
                        }

                        std::cout << "about to options.headerTransform.Call" << std::endl;

                        // TODO - this crashes without any information, what to do...
                        auto resultValue = options.headerTransform.Call( std::initializer_list<napi_value>{ static_cast<napi_value>( arr ) } );
                        std::cout << "just did options.headerTransform.Call" << std::endl;

                        if( resultValue.IsArray() )
                        {
                            auto resultObj = resultValue.ToObject();
                            auto resultArr = resultObj.As<Napi::Array>();
                            foundHeaders.clear();

                            for( int i = 0; i < resultArr.Length(); ++i )
                            {
                                auto something = resultArr.Get( i ).ToString();
                                foundHeaders.push_back( something.Utf8Value() );
                            }
                        }
                    }

                    for( int i = 0; i < foundHeaders.size(); ++i )
                    {
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
        else if( !info[2].IsObject() )
        {
            deferred.Reject( Napi::TypeError::New( env, "the third argument must be an object containing options").Value() );
            return deferred.Promise();
        }
        

        Napi::Function headerTransformFunc;

        if( info[2].ToObject().Has( "headerTransform" ) && info[2].ToObject().Get( "headerTransform" ).IsFunction() )
        {
            headerTransformFunc = info[2].ToObject().Get( "headerTransform" ).As<Napi::Function>();
        }

        const auto filename = info[0].ToString().Utf8Value();
        XlsxOptions opts;
        opts.headerTransform = headerTransformFunc;
        std::future<Napi::Array> fut = std::async( std::launch::async, getDataAsync, std::ref( env ), filename, info[1].ToBoolean(), opts );

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


    // int lettonum( const std::string& let )
    // {
    //     if( let.empty() )
    //     {
    //         return -1;
    //     }

    //     const std::string columnName = std::transform( let.begin(), let.end(), let.begin(), std::toupper );

    //     int sum = 0;

    //     for( int i = 0; i < columnName.size(); ++i )
    //     {
    //         sum *= 26;
    //         sum += ( columnName[i] - 'A' + 1 );
    //     }

    //     return sum;
    // }
}

