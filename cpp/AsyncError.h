#include <napi.h>

namespace xlsx
{
    // this class is used to enqueue an async error callback.
    // if we want to send an error to the client via the client's 
    // provided callback, then this is how we do it
    class AsyncError : public Napi::AsyncWorker
    {
    public:
        AsyncError( const Napi::Function& callback, const std::string& message );

    protected:
        void Execute() override;
        void OnOK() override;
        void OnError( const Napi::Error& e ) override;

    private:
        std::string myMessage;
    };
}
