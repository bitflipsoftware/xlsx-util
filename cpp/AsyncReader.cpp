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
    , myResult{}
    {

    }


    void
    AsyncReader::Execute()
    {
        myResult = extractAllData( myScope, myFilename, myHasHeaders, myHeaderTransform );
    }


    void
    AsyncReader::OnOK()
    {
        Callback().MakeCallback( Receiver().Value(), { myScope.Env().Null(), myResult } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
        Callback().MakeCallback( Receiver().Value(), { e.Value(), myScope.Env().Undefined() } );
    }
}
