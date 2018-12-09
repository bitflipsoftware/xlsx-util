#include "napi.h"
#include "Sheet.h"
#include <string>
#include <memory>

namespace xlsx
{
    // reads an xlsx file
    class AsyncReader : public Napi::AsyncWorker
    {
    public:// filename, hasHeaders, transformMap, columnsToDelete, doPascalCase, pascalWords, cb
        AsyncReader(
            const std::string& filename,
            bool hasHeaders,
            std::map<std::string, std::string> transformMap,
            std::set<std::string> deletes,
            bool doPascalCase,
            std::set<std::string> pascalWords,
            std::set<std::string> stringColumns,
            const Napi::Function& callback );

    protected:
        void Execute() override;
        void OnOK() override;
        void OnError( const Napi::Error& e ) override;

    private:
        std::string myFilename;
        bool myHasHeaders;
        std::map<std::string, std::string> myTransformMap;
        std::set<std::string> myDeletes;
        bool myDoPascalCase;
        std::set<std::string> myPascalWords;
        std::set<std::string> myStringColumns;
        Sheet mySheet;
    };
}
