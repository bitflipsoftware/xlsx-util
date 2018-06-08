#pragma once
#include <node.h>

namespace iq
{
	inline void toss( v8::Isolate* isolate, const char* const message )
	{
        const auto v8string = v8::String::NewFromUtf8( isolate, message );
        const auto ex = v8::Exception::Error( v8string );
        isolate->ThrowException( ex );
	}

	inline void tossTypeError( v8::Isolate* isolate, const char* const message )
	{
        const auto v8string = v8::String::NewFromUtf8( isolate, message );
        const auto ex = v8::Exception::TypeError( v8string );
        isolate->ThrowException( ex );
	}
}
