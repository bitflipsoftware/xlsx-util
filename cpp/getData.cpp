#include "napi.h"
#include "toss.h"
#include "str.h"
#include "xlsx.h"
#include "xlsxio_read.h"

#include <future>
#include <algorithm>
#include <sstream>
#include <string>

namespace iq
{
    std::string numtolet( int num );
    Napi::Value extractValue( Napi::Env& env, char*& cstr );
    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet );
    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool doSkipFirstRow, iq::XlsxReader& xreader );

    Napi::Array getDataAsync( Napi::Env& env, const std::string& filename )
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
        returnArr[static_cast<uint32_t>( 0 )] = extractAllRows( env, sheetname, false, xreader );
        return returnArr;
    }


    Napi::Array extractAllRows( Napi::Env& env, const char* sheetname, bool doSkipFirstRow, iq::XlsxReader& xreader )
    {
        auto returnArr = Napi::Array::New( env );
        xlsxioreadersheet sheet = nullptr;
        int rowIndex = 0;

        if( ( sheet = xlsxioread_sheet_open( xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != NULL )
        {
            //read all rows
            while( xlsxioread_sheet_next_row( sheet ) )
            {
                returnArr[static_cast<uint32_t>( rowIndex )] = extractRow( env, sheet );
                ++rowIndex;
            }

            xlsxioread_sheet_close(sheet);
        }

        return returnArr;
    }


    Napi::Object extractRow( Napi::Env& env, xlsxioreadersheet sheet )
    {
        char* valueCstr = nullptr;
        int cellIndex = 0;
        auto row = Napi::Object::New( env );
        
        //read all columns
        while( ( valueCstr = xlsxioread_sheet_next_cell( sheet ) ) != NULL )
        {
            auto napiString = extractValue( env, valueCstr );

            // TODO - convert numbers to numbers
            // TODO - convert scientific notation to number
            const auto cellIndexNapi = Napi::String::New( env, numtolet( cellIndex + 1 ) );
            row.Set(cellIndexNapi, napiString);
            ++cellIndex;
        }

        return row;
    }


    Napi::Value extractValue( Napi::Env& env, char*& cstr )
    {
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

        const auto filename = info[0].ToString().Utf8Value();

        std::future<Napi::Array> fut = std::async( std::launch::async, getDataAsync, std::ref( env ), filename );

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

