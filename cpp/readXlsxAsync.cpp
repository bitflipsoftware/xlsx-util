#include "napi.h"
#include "readXlsxAsync.h"
#include "AsyncReader.h"
#include "AsyncError.h"

namespace xlsx
{
    void readXlsxAsync( const Napi::CallbackInfo& info )
    {
        Napi::Env env = info.Env();
        Napi::HandleScope scope{ env };

        // we need to check for the presence of a client callback function
        // and if we do not have one then we must raise a synchronous error
        if( info.Length() != 2 )
        {
            Napi::TypeError::New(env, "invalid argument count").ThrowAsJavaScriptException();
            return;
        }

        if( !info[1].IsFunction() )
        {
            Napi::TypeError::New(env, "invalid callback function [arg1]").ThrowAsJavaScriptException();
            return;
        }

        Napi::Function cb = info[1].As<Napi::Function>();

        if( !info[0].IsString() )
        {
            auto err = new AsyncError{ cb, "first argument must be a string [filename]" };
            err->Queue();
            return;
        }

        const std::string filename = info[0].As<Napi::String>().Utf8Value();
        AsyncReader* reader = new AsyncReader{ filename, cb };
        reader->Queue();
    }
}
