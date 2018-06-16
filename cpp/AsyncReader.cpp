#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"
#include "numtolet.h"

namespace xlsx
{
    AsyncReader::AsyncReader( const std::string& filename, bool hasHeaders, Napi::Function transform, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myFilename{ filename }
    , myHasHeaders{ hasHeaders }
    , myTransform{ transform }
    , mySheet{}
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
        Napi::Array arr = Napi::Array::New( Env() );
        const int numRows = mySheet.getNumRows();

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
