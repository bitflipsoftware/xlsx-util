#include "napi.h"
#include <string>

namespace xlsx
{
    // reads an xlsx file
    class AsyncReader : public Napi::AsyncWorker
    {
    public:
        AsyncReader( const std::string& filename, bool hasHeaders, const Napi::Function& callback );

    protected:
        void Execute() override;
        void OnOK() override;
        void OnError( const Napi::Error& e ) override;

    private:
        std::string myFilename;
        bool myHasHeaders;
        std::vector<std::vector<std::string>> myData;
    };
}
