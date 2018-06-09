#include "xlsx.h"
#include <string>
#include <vector>

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


	std::vector<std::string>
	XlsxReader::getSheetNames() const
	{
		std::vector<std::string> result;

		if( !getIsOk() )
		{
			return result;
		}

		struct xlsx_list_sheets_data sheetdata;
      // printf("Available sheets:\n");
      // sheetdata.index = 0;
      // sheetdata.firstsheet = NULL;
      xlsxioread_list_sheets(myReader, xlsx_list_sheets_callback, &sheetdata);
      // printf("Sheets found: %i\n", sheetdata.index);
		// const char* sheetname = nullptr;
		// if ( ( sheet = xlsxioread_sheet_open(myReader, sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS ) ) != nullptr )
		// {
		// 	if( sheetname != nullptr )
		// 	{
		// 		result.push_back( std::string{ sheetname } );
		// 	}
		// }
      return result;
	}


	bool
	XlsxReader::getIsOk() const
	{
		return myIsOk;
	}
}
