#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"

namespace xlsx
{
    AsyncReader::AsyncReader( Napi::HandleScope scope, const std::string& filename, bool hasHeaders, const Napi::Function& headerTransform, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myScope{ scope }
    , myFilename{ filename }
    , myHasHeaders{ hasHeaders }
    , myHeaderTransform{ headerTransform }
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
                                                       // error              // result
        Callback().MakeCallback( Receiver().Value(), { myScope.Env().Null(), myScope.Env().Null() } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
                                                       // error   // result
        Callback().MakeCallback( Receiver().Value(), { e.Value(), myScope.Env().Undefined() } );
    }
}
