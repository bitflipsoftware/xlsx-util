#include "napi.h"
#include "Sheet.h"
#include <string>

namespace xlsx
{
    // reads an xlsx file
    class AsyncReader : public Napi::AsyncWorker
    {
    public:
        AsyncReader( const std::string& filename, bool hasHeaders, Napi::Function transform, std::set<std::string> deletes, const Napi::Function& callback );

    protected:
        void Execute() override;
        void OnOK() override;
        void OnError( const Napi::Error& e ) override;

    private:
        std::string myFilename;
        bool myHasHeaders;
        Napi::Function myTransform;
        std::set<std::string> myDeletes;
        Sheet mySheet;
    };
}
