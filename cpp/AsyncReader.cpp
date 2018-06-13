#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"

namespace xlsx
{
    AsyncReader::AsyncReader( const std::string& filename, bool hasHeaders, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myFilename{ filename }
    , myHasHeaders{ hasHeaders }
    , myData{}
    {

    }

    void
    AsyncReader::Execute()
    {
        myData = extractAllData( myFilename );
    }


    void
    AsyncReader::OnOK()
    {
                                                       // error      // result
        Callback().MakeCallback( Receiver().Value(), { Env().Null(), Env().Null() } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
                                                       // error   // result
        Callback().MakeCallback( Receiver().Value(), { e.Value(), Env().Undefined() } );
    }
}
