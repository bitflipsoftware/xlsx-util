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
		std::vector<std::string> getSheetNames() const;
	private:
		bool myIsOk;
		std::string myFilename;
		xlsxioreader myReader;

	private:
		struct xlsx_list_sheets_data {
		  std::vector<std::string> sheets;
		};

		static int xlsx_list_sheets_callback (const char* name, void* callbackdata)
		{
		  struct xlsx_list_sheets_data* data = (struct xlsx_list_sheets_data*)callbackdata;
		  // if (!data->firstsheet)
		  //   data->firstsheet = strdup(name);
		  // printf("%3i: %s\n", data->index, name);
		  // data->index++;
		  data->sheets.push_back( std::string{ name } );
		  return 0;
		}
	};
}
