#include <node.h>

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


    void init( Local<Object> exports )
    {
        NODE_SET_METHOD( exports, "checkLibrary", checkLibrary );
        NODE_SET_METHOD( exports, "getSheetNames", getSheetNames );
    }

    NODE_MODULE( NODE_GYP_MODULE_NAME, init )

}
