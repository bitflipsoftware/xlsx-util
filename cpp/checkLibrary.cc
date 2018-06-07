// hello.cc
#include <node.h>
#include <xlsxio_read.h>
#include <string>
#include <vector>

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

if (args.Length() != 1) {
    isolate->ThrowException(v8::Exception::TypeError(
        String::NewFromUtf8(isolate, "The filename is required.")));
    return;
  }

// Check the argument types
  if (!args[0]->IsString()) {
    isolate->ThrowException(v8::Exception::TypeError(
        String::NewFromUtf8(isolate, "The filename must be a string.")));
    return;
  }

    std::string filename{ *v8::String::Utf8Value(args[0]->ToString()) };

  // The purpose of this file and function is just to check that the compilation
  // of xlsxio occurred successfully and if we try to access a function inside
  // that library that we can do so without crashing.

    //open .xlsx file for reading
    xlsxioreader xlsxioread;
    if ((xlsxioread = xlsxioread_open(filename.c_str())) == NULL) {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, "fail"));
          isolate->ThrowException(v8::Exception::Error(
              String::NewFromUtf8(isolate, "Unable to open xlsx file.")));
          return;
    }

    std::vector<std::string> valuesFound;

    //read values from first sheet
    char* value;
    xlsxioreadersheet sheet;
    const char* sheetname = NULL;
    if ((sheet = xlsxioread_sheet_open(xlsxioread, sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS)) != NULL) {
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
    xlsxioread_close(xlsxioread);

    // create an object to pass the values back
    Local<Object> obj = Object::New(isolate);
    for( const auto& v : valuesFound )
    {
        obj->Set(String::NewFromUtf8(isolate, "msg"), args[0]->ToString());
    }

  args.GetReturnValue().Set(obj);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "checkLibrary", Method);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace demo
