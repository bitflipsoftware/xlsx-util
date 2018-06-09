// hello.cc
#include "toss.h"
#include "str.h"
#include "xlsx.h"
#include <node.h>
#include <xlsxio_read.h>
#include <string>
#include <vector>

namespace demo
{

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void checkLibrary( const FunctionCallbackInfo<Value>& args )
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

    std::vector<std::string> valuesFound;

    const auto x = xreader.getSheetNames();
    //read values from first sheet
    char* value;
    xlsxioreadersheet sheet;
    const char* sheetname = NULL;
    if ((sheet = xlsxioread_sheet_open(xreader.getReader(), sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS)) != NULL) {
      //read all rows
      while (xlsxioread_sheet_next_row(sheet)) {
        //read all columns
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
          valuesFound.push_back( std::string{ value } );
          free(value);
        }
        printf("\n");
      }
      xlsxioread_sheet_close(sheet);
    }

    //clean up
    //xlsxioread_close(xlsxioread);

    // create an object to pass the values back
    Local<Object> obj = Object::New(iso);
    for( const auto& v : valuesFound )
    {
        obj->Set(String::NewFromUtf8(iso, "msg"), args[0]->ToString());
    }

  args.GetReturnValue().Set(obj);
}

void init( Local<Object> exports )
{
    NODE_SET_METHOD(exports, "checkLibrary", checkLibrary);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace demo
