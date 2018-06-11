#include "napi.h"
#include "toss.h"
#include "str.h"
#include "xlsx.h"
#include "xlsxio_read.h"

Napi::Promise getSheetNames( const Napi::CallbackInfo& info )
{
    Napi::Env env = info.Env();

    auto deferred = Napi::Promise::Deferred::New(env);

    if ( info.Length() != 1 )
    {
        deferred.Reject( Napi::TypeError::New( env, "one argument is required: filename").Value() );
        return deferred.Promise();
    }
    else if( !info[0].IsString()  )
    {
        deferred.Reject( Napi::TypeError::New( env, "one string argument is required: filename").Value() );
        return deferred.Promise();
    }

    const std::string filename = info[0].ToString().Utf8Value();
    iq::XlsxReader xreader{ filename };

    if( !xreader.getIsOk() )
    {
        deferred.Reject( Napi::TypeError::New( env, "xlsx file could not be opened" ).Value() );
        return deferred.Promise();
    }

    const auto sheetNames = xreader.getSheetNames();
    auto returnArr = Napi::Array::New( env );

    for( int index = 0; index < sheetNames.size(); ++index )
    {
        const auto& name = sheetNames.at( index );
        auto napiString = Napi::String::New( env, name );
        returnArr[index] = napiString;
    }

    deferred.Resolve( returnArr );
    return deferred.Promise();
}

Napi::Object Init( Napi::Env env, Napi::Object exports )
{
  exports.Set(
    Napi::String::New( env, "getSheetNames"),
    Napi::Function::New(env, getSheetNames)
  );

  return exports;
}


NODE_API_MODULE(xlsx, Init)