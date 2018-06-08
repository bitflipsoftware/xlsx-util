#pragma once
#include "toss.h"
#include "str.h"
#include <node.h>
#include <xlsxio_read.h>
#include <string>
#include <vector>

namespace iq
{
	class XlsxReader
	{
	public:
		XlsxReader( const std::string& inFilename );
		~XlsxReader();

	public:
		xlsxioreader getReader() const;
		bool getIsOk() const;

	private:
		bool myIsOk;
		std::string myFilename;
		xlsxioreader myReader;
	};
}
