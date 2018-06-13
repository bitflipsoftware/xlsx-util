#include "AsyncReader.h"

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
        // TODO - everything 
    }


    void
    AsyncReader::OnOK()
    {
        // return successful values here
        this->Callback().MakeCallback( this->Receiver().Value(), { this->Env().Null(), myResult } );
    }


    void
    AsyncReader::OnError( const Napi::Error& e )
    {
        // return error here
        this->Callback().MakeCallback( this->Receiver().Value(), { e.Value(), this->Env().Undefined() } );
    }
}
