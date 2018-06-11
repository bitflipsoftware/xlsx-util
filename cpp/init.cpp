#include "node.h"
#include "napi.h"

namespace iq
{

    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    extern void getSheetNames( const FunctionCallbackInfo<Value>& args );
    extern void checkLibrary( const FunctionCallbackInfo<Value>& args );
    extern Napi::Promise SumAsyncPromise( const Napi::CallbackInfo& info );

    Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
    Napi::String::New(env, "add"),
    Napi::Function::New(env, SumAsyncPromise)
  );

  exports.Set(
    Napi::String::New(env, "checkLibrary"),
    Napi::Function::New(env, checkLibrary)
  );

  exports.Set(
    Napi::String::New(env, "getSheetNames"),
    Napi::Function::New(env, getSheetNames)
  );

  // NODE_SET_METHOD( exports, "checkLibrary", checkLibrary );
  // NODE_SET_METHOD( exports, "getSheetNames", getSheetNames );
  return exports;
}


NODE_API_MODULE( addon, Init )

}

