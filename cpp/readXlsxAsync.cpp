#include "readXlsxAsync.h"
#include "AsyncReader.h"

namespace xlsx
{
    void readXlsxAsync( const Napi::CallbackInfo& info )
    {
        Napi::Env env = info.Env();

        // we need to check for the presence of a client callback function
        // and if we do not have one then we must raise a synchronous error
        if( info.Length() != 4 )
        {
            Napi::TypeError::New(env, "invalid argument count").ThrowAsJavaScriptException();
            return;
        }

        if( !info[3].IsFunction() )
        {
            Napi::TypeError::New(env, "invalid callback function [arg3]").ThrowAsJavaScriptException();
            return;
        }

        Napi::Function cb = info[3].As<Napi::Function>();

        if( !info[0].IsString() )
        {
            auto err = new ErrorAsyncWorker{ cb, "first argument must be a string [filename]" }
            err->Queue();
            return;
        }

        if( !info[1].IsBoolean() )
        {
            auto err = new ErrorAsyncWorker{ cb, "second argument must be a bool [hasHeaders]" }
            err->Queue();
            return;
        }

        if( !info[2].IsFunction() )
        {
            auto err = new ErrorAsyncWorker{ cb, "third argument must be a function [headerTransform]" }
            err->Queue();
            return;
        }

        const std::string filename = info[0].As<Napi::String>.Utf8Value();
        const bool hasHeaders = info[1].As<Napi::Boolean>;
        Napi::Function headerTransform = info[2].As<Napi::Function>;
        AsyncReader* reader = new AsyncReader{ filename, hasHeaders, headerTransform, cb };
        reader->Queue();
    }
}