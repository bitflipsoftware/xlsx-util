// hello.cc
#include <node.h>
#include <xlsxio_read.h>

namespace demo {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // The purpose of this file and function is just to check that the compilation
  // of xlsxio occurred successfully and if we try to access a function inside
  // that library that we can do so without crashing.

    //open .xlsx file for reading
    xlsxioreader xlsxioread;
    if ((xlsxioread = xlsxioread_open("FILENAME.XLSX")) == NULL) {
      args.GetReturnValue().Set(String::NewFromUtf8(isolate, "fail"));
      fprintf(stderr, "Error opening .xlsx file\n");
      return;
    }

    //read values from first sheet
    char* value;
    xlsxioreadersheet sheet;
    const char* sheetname = NULL;
    printf("Contents of first sheet:\n");
    if ((sheet = xlsxioread_sheet_open(xlsxioread, sheetname, XLSXIOREAD_SKIP_EMPTY_ROWS)) != NULL) {
      //read all rows
      while (xlsxioread_sheet_next_row(sheet)) {
        //read all columns
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
          printf("%s\t", value);
          free(value);
        }
        printf("\n");
      }
      xlsxioread_sheet_close(sheet);
    }

    //clean up
    xlsxioread_close(xlsxioread);

  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "we survived"));
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "checkLibrary", Method);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)

}  // namespace demo
