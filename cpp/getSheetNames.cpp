// hello.cc
#include "toss.h"
#include "str.h"
#include "xlsx.h"
#include <node.h>
#include <xlsxio_read.h>
#include <string>
#include <vector>

namespace iq
{

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void getSheetNames( const FunctionCallbackInfo<Value>& args )
{

    Isolate* iso = args.GetIsolate();

    // check to see that we have exactly one argument
    if( args.Length() != 1 )
    {
        iq::tossTypeError( iso, "Exactly one argument is required, the filename." );
        return;
    }

    // Check the argument types
    if ( !args[0]->IsString() )
    {
        iq::tossTypeError( iso, "The filename must be a string." );
        return;
    }

    const std::string filename = iq::str( args[0] );

    iq::XlsxReader xreader{ filename };

    if( !xreader.getIsOk() )
    {
        iq::toss( iso, "Unable to open xlsx file." );
        return;
    }

    const auto sheetNames = xreader.getSheetNames();
    Local<v8::Array> returnArr = v8::Array::New( iso );

    for( int index = 0; index < sheetNames.size(); ++index )
    {
        const auto& name = sheetNames.at( index );
//        v8::Handle<Value> v8Val = v8::String::New( name.c_str() );
        v8::Local<v8::String> s = v8::String::NewFromUtf8( iso, name.c_str() );
//        Local<Object> nodeObj = Object::New( iso );
//        pack_rain_result(iso, result, results[i]);
        returnArr->Set( index, s );
    }

  args.GetReturnValue().Set(returnArr);
}


}
