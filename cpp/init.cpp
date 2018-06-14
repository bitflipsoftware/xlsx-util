#include "napi.h"
#include "readXlsxAsync.h"

namespace xlsx
{
    Napi::Object Init( Napi::Env env, Napi::Object exports )
    {
      auto readXlsxAsyncStr = Napi::String::New( env, "readXlsxAsync");
      auto readXlsxAsyncFun = Napi::Function::New(env, readXlsxAsync);
      exports.Set( readXlsxAsyncStr, readXlsxAsyncFun );
      return exports;
    }

    NODE_API_MODULE( xlsx, Init );
}
