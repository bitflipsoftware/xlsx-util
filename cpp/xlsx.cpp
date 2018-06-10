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


    int
    XlsxReader::xlsx_list_sheets_callback( const char* name, void* callbackdata )
    {
      struct xlsx_list_sheets_data* data = (struct xlsx_list_sheets_data*)callbackdata;
      // if (!data->firstsheet)
      //   data->firstsheet = strdup(name);
      // printf("%3i: %s\n", data->index, name);
      // data->index++;
      data->sheets.push_back( std::string{ name } );
      return 0;
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
        xlsxioread_list_sheets(myReader, xlsx_list_sheets_callback, &sheetdata);
        return result;
    }


    bool
    XlsxReader::getIsOk() const
    {
        return myIsOk;
    }
}
