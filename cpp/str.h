#pragma once
#include <node.h>
#include <string>

namespace iq
{
	template <typename NODE_THING>
	inline std::string str( NODE_THING val )
	{
        return std::string{ *v8::String::Utf8Value( val->ToString() ) };
	}
}