#include "AsyncError.h"

namespace xlsx
{
    AsyncError::AsyncError( const Napi::Function& callback, const std::string& message )
    : Napi::AsyncWorker{ callback }
    , error{ error }
    {

    }


    void
    AsyncError::Execute()
    {
        // do nothing, we will send an error OnOK
    }


    void
    AsyncError::OnOK()
    {
        Napi::Env env = Env();
        auto error = Napi::TypeError::New( env, myMessage );
        Callback().MakeCallback( Receiver().Value(), { error.Value(), env.Undefined() } );
    }


    void
    AsyncError::OnError( const Napi::Error& e )
    {
        Napi::Env env = Env();
        Callback().MakeCallback( Receiver().Value(), { e.Value(), env.Undefined() } );
    }
}
