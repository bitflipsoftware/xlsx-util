#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"
#include "numtolet.h"
#include <iostream>

namespace xlsx
{
    AsyncReader::AsyncReader( std::unique_ptr<Napi::EscapableHandleScope>&& scope, const std::string& filename, bool hasHeaders, Napi::Function transform, std::set<std::string> deletes, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myFilename{ filename }
    , myHasHeaders{ hasHeaders }
    , myTransform{ transform }
    , myDeletes{ std::move( deletes ) }
    , mySheet{}
    , myScopeUptr{ new Napi::EscapableHandleScope{ callback.Env() } }
    {
        
    }
    

    void
    AsyncReader::Execute()
    {
        try
        {
            mySheet = extractAllData( myFilename, myHasHeaders );
        }
        catch( std::exception& ex )
        {
            SetError( ex.what() );
        }
        catch( ... )
        {
            SetError( "xlsx-util: an unknown exception occurred during 'myData = extractAllData( myFilename );'");
        }
    }


    void
    AsyncReader::OnOK()
    {
        if( myTransform.IsNumber() )
        {
            throw std::runtime_error{ "myTransform is a number" };
        }
        else if( myTransform.IsNull() )
        {
            throw std::runtime_error( "myTransform is null" );
        }
        else if( myTransform.IsUndefined() )
        {
            throw std::runtime_error( "myTransform is undefined" );
        }
        else if( myTransform.IsFunction() )
        {
            throw std::runtime_error( "myTransform is function" );
        }
        
        // myTransform = Napi::Value{ Env(), myTransform }.As<Napi::Function>();
        Napi::Array arr = Napi::Array::New( Env() );
        const int numRows = mySheet.getNumRows();
        auto headers = mySheet.getHeaders();

        if( !myTransform.IsNull() && !myTransform.IsUndefined() )
        {
            Napi::Array headersJs = Napi::Array::New( Env() );
            for( size_t x = 0; x < headers.size(); ++ x )
            {
                headersJs[x] = Napi::String::New( Env(), headers.at( x ) );
            }

            std::cout << "before: auto transformedVal = myTransform.Call( { headersJs } );" << std::endl;
            auto transformedVal = myTransform.Call( { headersJs } );
            std::cout << "after: auto transformedVal = myTransform.Call( { headersJs } );" << std::endl;

            if( transformedVal.IsArray() )
            {
                auto transformedArr = transformedVal.As<Napi::Array>();
                const auto transformedLen = transformedArr.Length();

                if( transformedLen == headers.size() )
                {
                    for( size_t z = 0; z < transformedLen; ++z )
                    {
                        auto h = transformedArr.Get( z );
                        if( h.IsString() )
                        {
                            headers.at( z ) = h.As<Napi::String>().Utf8Value();
                        }
                    }
                }
            }
        }


        for( int i = 0; i < numRows; ++i )
        {
            const auto row = mySheet.getRow( i );
            Napi::Object obj = Napi::Object::New( Env() );

            for( size_t j = 0; j < row.size(); ++j )
            {
                const auto val = row.at( j );
                const auto let = mySheet.getHeaders().at( j );
                
                if( val.getIsString() )
                {
                    obj.Set( Napi::String::New( Env(), let ), Napi::String::New( Env(), val.getString() ) );
                }
                else if( val.getIsInt() )
                {
                    obj.Set( Napi::String::New( Env(), let ), Napi::Number::New( Env(), static_cast<double>( val.getInt() ) ) );
                }
                else if( val.getIsDouble() )
                {
                    obj.Set( Napi::String::New( Env(), let ), Napi::Number::New( Env(), val.getDouble() ) );
                }
                else
                {
                    obj.Set( Napi::String::New( Env(), let ), Napi::Value{ Env(), Env().Null() } );
                }
            }

            arr[i] = obj;
        }
                                                       // error      // result
        Callback().MakeCallback( Receiver().Value(), { Env().Null(), arr } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
                                                       // error   // result
        Callback().MakeCallback( Receiver().Value(), { e.Value(), Env().Undefined() } );
    }
}
