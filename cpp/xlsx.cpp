#include "xlsx.h"

namespace iq
{
	XlsxReader::XlsxReader( const std::string& inFilename )
	: myIsOk{ false }
	, myFilename{ inFilename }
	, myReader{ nullptr }
	{
		myReader = xlsxioread_open( myFilename.c_str() );

		if( myReader != nullptr )
		{
			myIsOk = true;
		}
	}


	XlsxReader::~XlsxReader()
	{
		if( myIsOk )
		{
			if( myReader )
			{
				xlsxioread_close( myReader );
			}
		}
	}


	xlsxioreader
	XlsxReader::getReader() const
	{
		return myReader;
	}


	bool
	XlsxReader::getIsOk() const
	{
		return myIsOk;
	}
}
