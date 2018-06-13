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
    // int lettonum( const std::string& let );
    Napi::Value extractValue( Napi::Env& env, char*& cstr );
    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet, std::map<int, std::string>& ioHeaders );
    std::vector<std::string> extractHeaders( Napi::Env& env, xlsxioreadersheet sheet );
    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders, Napi::Function& transform );
    std::string findHeaderName( int columnIndex, std::map<int, std::string>& ioHeaders );


    Napi::Array getDataAsync( Napi::Env& env, const std::string& filename, bool hasHeaders, Napi::Function& transform )
    {
        std::cout << "begin getDataAsync" << std::endl;
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
        returnArr[static_cast<uint32_t>( 0 )] = extractAllRows( env, sheetname, hasHeaders, xreader, headers, transform );
        return returnArr;
    }


    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool hasHeaders, iq::XlsxReader& xreader, std::map<int, std::string>& ioHeaders, Napi::Function& transform )
    {
        std::cout << "begin extractAllRows" << std::endl;
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
                    
                    if( !transform.IsNull() && !foundHeaders.empty() )
                    {
                        auto arr = Napi::Array::New( env );

                        for( int i = 0; i < foundHeaders.size(); ++i )
                        {
                            arr[i] = Napi::String::New( env, foundHeaders.at( i ) );
                            std::cout << foundHeaders.at( i ) << std::endl;
                        }

                        std::cout << "about to transform.Call" << std::endl;

                        // TODO - this crashes without any information, what to do...
                        const std::initializer_list<napi_value> myArgs{ Napi::Array::New( env ) };
                        // auto resultValue = transform.Call( env.Global(), myArgs );
                        std::cout << "just did transform.Call" << std::endl;

                        // if( resultValue.IsArray() )
                        // {
                        //     auto resultObj = resultValue.ToObject();
                        //     auto resultArr = resultObj.As<Napi::Array>();
                        //     foundHeaders.clear();

                        //     for( int i = 0; i < resultArr.Length(); ++i )
                        //     {
                        //         auto something = resultArr.Get( i ).ToString();
                        //         foundHeaders.push_back( something.Utf8Value() );
                        //     }
                        // }
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
        std::cout << "begin findHeaderName" << std::endl;
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
        std::cout << "begin extractRow" << std::endl;
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
        std::cout << "begin extractHeaders" << std::endl;
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
        std::cout << "begin extractValue" << std::endl;
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
        std::cout << "begin getData" << std::endl;
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
        else if( !info[2].IsFunction() )
        {
            deferred.Reject( Napi::TypeError::New( env, "the third argument must be an object which takes an array or strings and returns an array of string").Value() );
            return deferred.Promise();
        }
        

        auto headerTransformFunc = info[2].ToObject().As<Napi::Function>();

        const std::initializer_list<napi_value> myArgs{ Napi::Array::New( env ) };
        auto x = info[2].ToObject().As<Napi::Function>().Call( env.Global(), myArgs );
        // auto resultValue = transform.Call(  );

        const auto filename = info[0].ToString().Utf8Value();
        std::future<Napi::Array> fut = std::async( std::launch::async, getDataAsync, std::ref( env ), filename, info[1].ToBoolean(), std::ref( headerTransformFunc ) );

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

