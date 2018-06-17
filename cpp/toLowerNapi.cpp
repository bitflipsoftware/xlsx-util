#include "napi.h"
#include "toLowerNapi.h"
#include "toLower.h"
#include <string>
#include <sstream>

// filepathString, doReadHeaders, headerTransformObj, columnsToDeleteArr, doPascalCase, pascalWords, (error, result) =>

namespace xlsx
{
    Napi::String
    toLowerNapi( const Napi::CallbackInfo& info )
    {
        Napi::Env env = info.Env();
        const int expectedNumberOfArguments = 1;

        if( info.Length() != expectedNumberOfArguments )
        {
            Napi::TypeError::New(env, "xlsx-util: invalid argument count - should be 1").ThrowAsJavaScriptException();
            return Napi::String::New( env, "error" );
        }

        for( int i = 0; i < expectedNumberOfArguments; ++i )
        {
            auto arg = info[i];
            if( !arg.IsString() )
            {
                std::stringstream msg;
                msg << "xlsx-util: invalid argument type, should be string, arg[" << i << "]";
                Napi::TypeError::New(env, msg.str()).ThrowAsJavaScriptException();
                return Napi::String::New( env, "error" );
            }
        }

        const std::string input = info[0].As<Napi::String>().Utf8Value();
        const std::string result = toLower( input );
        return Napi::String::New( env, result );
    }
}
