#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"
#include "numtolet.h"

namespace xlsx
{
    AsyncReader::AsyncReader( const std::string& filename, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myFilename{ filename }
    , myData{}
    {

    }
    

    void
    AsyncReader::Execute()
    {
        try
        {
            myData = extractAllData( myFilename );
        }
        catch( std::exception& ex )
        {
            SetError( ex.what() );
        }
        catch( ... )
        {
            SetError( "an unknown exception occurred during 'myData = extractAllData( myFilename );'");
        }
    }


    void
    AsyncReader::OnOK()
    {
        Napi::Array arr = Napi::Array::New( Env() );

        for( size_t i = 0; i < myData.size(); ++i )
        {
            const auto row = myData.at( i );
            Napi::Object obj = Napi::Object::New( Env() );

            for( size_t j = 0; j < row.size(); ++j )
            {
                const auto val = row.at( j );
                const auto let = numtolet( j + 1 );
                obj.Set( Napi::String::New( Env(), let ), Napi::String::New( Env(), val ) );
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
