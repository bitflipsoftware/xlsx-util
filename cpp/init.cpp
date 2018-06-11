#include "napi.h"

namespace iq
{
    extern Napi::Promise getSheetNames( const Napi::CallbackInfo& info );
    extern Napi::Promise getData( const Napi::CallbackInfo& info );

    Napi::Object Init( Napi::Env env, Napi::Object exports )
    {
      exports.Set(
        Napi::String::New( env, "getSheetNames"),
        Napi::Function::New(env, getSheetNames)
      );

      exports.Set(
        Napi::String::New( env, "getData"),
        Napi::Function::New(env, getData)
      );

      return exports;
    }


    NODE_API_MODULE( xlsx, Init );
}

