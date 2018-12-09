#include "napi.h"
#include "readXlsxAsync.h"
#include "replaceAllNapi.h"
#include "toLowerNapi.h"

namespace xlsx
{
    Napi::Object Init( Napi::Env env, Napi::Object exports )
    {
      auto funStr = Napi::String::New( env, "readXlsxAsync");
      auto funObj = Napi::Function::New( env, readXlsxAsync );
      exports.Set( funStr, funObj );

      funStr = Napi::String::New( env, "replaceAll");
      funObj = Napi::Function::New( env, replaceAllNapi );
      exports.Set( funStr, funObj );

      funStr = Napi::String::New( env, "toLower");
      funObj = Napi::Function::New( env, toLowerNapi );
      exports.Set( funStr, funObj );

      return exports;
    }

    NODE_API_MODULE( xlsx, Init );
}
