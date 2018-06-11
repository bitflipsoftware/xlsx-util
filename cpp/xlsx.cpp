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
    XlsxReader::listSheetsCallback( const char* name, void* callbackdata )
    {
      struct SheetNames* data = static_cast<SheetNames*>( callbackdata );
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

        struct SheetNames sheetdata;
        xlsxioread_list_sheets( myReader, listSheetsCallback, &sheetdata );
        result = sheetdata.sheets;
        return result;
    }


    bool
    XlsxReader::getIsOk() const
    {
        return myIsOk;
    }
}
