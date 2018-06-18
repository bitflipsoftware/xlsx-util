#include "napi.h"
#include "readXlsxAsync.h"
#include "AsyncReader.h"
#include "AsyncError.h"
#include "normalizeString.h"
#include <set>

// filepathString, doReadHeaders, headerTransformObj, columnsToDeleteArr, doPascalCase, pascalWords, (error, result) =>

namespace xlsx
{
    void readXlsxAsync( const Napi::CallbackInfo& info )
    {
        Napi::Env env = info.Env();

        // we need to check for the presence of a client callback function
        // and if we do not have one then we must raise a synchronous error
        if( info.Length() != 7 )
        {
            Napi::TypeError::New(env, "xlsx-util: invalid argument count - should be 7").ThrowAsJavaScriptException();
            return;
        }

        if( !info[6].IsFunction() )
        {
            Napi::TypeError::New(env, "xlsx-util: invalid callback function [arg6]").ThrowAsJavaScriptException();
            return;
        }

        Napi::Function cb = info[6].As<Napi::Function>();

        if( !info[0].IsString() )
        {
            auto err = new AsyncError{ cb, "xlsx-util: [arg0] must be a string [filename]" };
            err->Queue();
            return;
        }

        const std::string filename = info[0].As<Napi::String>().Utf8Value();

        bool hasHeaders = false;

        if( !info[1].IsUndefined() && !info[1].IsNull() && info[1].IsBoolean() )
        {
            hasHeaders = info[1].ToBoolean();
        }

        std::map<std::string, std::string> transformMap;

        if( !info[2].IsUndefined() && !info[2].IsNull() && info[2].IsObject() )
        {
            auto obj = info[2].As<Napi::Object>();
            auto props = obj.GetPropertyNames();
            auto propsLen = props.Length();

            for( decltype(propsLen) p = 0; p < propsLen; ++p )
            {
                auto keyNapi = props.Get( p ).As<Napi::String>();
                const std::string key = keyNapi.Utf8Value();
                auto valNapiValue = obj.Get( keyNapi );
                if( valNapiValue.IsString() )
                {
                    auto valNapi = valNapiValue.As<Napi::String>();
                    const std::string val = valNapi.Utf8Value();
                    if( !key.empty() && !val.empty() )
                    {
                        transformMap[normalizeString( key )] = val;
                    }
                }
            }
        }

        std::set<std::string> columnsToDelete;

        if( !info[3].IsUndefined() && !info[3].IsNull() && info[3].IsArray() )
        {
            auto deletes = info[3].As<Napi::Array>();

            for( int i = 0; i < deletes.Length(); ++i )
            {
                auto value = deletes.Get( i );
                if( value.IsString() )
                {
                    columnsToDelete.insert( value.As<Napi::String>().Utf8Value() );
                }
            }
        }

        std::set<std::string> pascalWords;

        if( !info[5].IsUndefined() && !info[5].IsNull() && info[5].IsArray() )
        {
            auto arr = info[5].As<Napi::Array>();

            for( int i = 0; i < arr.Length(); ++i )
            {
                auto value = arr.Get( i );
                if( value.IsString() )
                {
                    pascalWords.insert( value.As<Napi::String>().Utf8Value() );
                }
            }
        }

        bool doPascalCase = false;

        if( !info[4].IsUndefined() && !info[4].IsNull() && info[4].IsBoolean() )
        {
            doPascalCase = info[4].ToBoolean();
        }

        AsyncReader* reader = new AsyncReader{ filename, hasHeaders, transformMap, columnsToDelete, doPascalCase, pascalWords, cb };
        reader->Queue();
    }
}
