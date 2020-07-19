#ifndef SRC_ERROR_HANDLER_HPP_
#define SRC_ERROR_HANDLER_HPP_

#include <proxygen/httpserver/RequestHandler.h>

namespace ggml { namespace handler {

class ErrorHandler : public proxygen::RequestHandler {
public:
    ErrorHandler(int code, const char* message);

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;

private:
    int m_code;
    const char* m_message;
};

} // namespace handler
} // namespace ggml

#endif // SRC_ERROR_HANDLER_HPP_
