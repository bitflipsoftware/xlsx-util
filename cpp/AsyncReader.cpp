#include "AsyncReader.h"
#include "XlsxReaderFunctions.h"

namespace xlsx
{
    AsyncReader::AsyncReader( const std::string& filename, bool hasHeaders, const Napi::Function& headerTransform, const Napi::Function& callback )
    : Napi::AsyncWorker{ callback }
    , myFilename{ filename }
    , myHasHeaders{ hasHeaders }
    , myHeaderTransform{ headerTransform }
    , myResult{}
    {

    }


    void
    AsyncReader::Execute()
    {
        myResult = extractAllData( this->Env(), myFilename, myHasHeaders, myHeaderTransform );
    }


    void
    AsyncReader::OnOK()
    {
        this->Callback().MakeCallback( this->Receiver().Value(), { this->Env().Null(), myResult } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
        this->Callback().MakeCallback( this->Receiver().Value(), { e.Value(), this->Env().Undefined() } );
    }
}
