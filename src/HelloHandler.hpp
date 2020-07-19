#ifndef SRC_HELLO_HANDLER_HPP_
#define SRC_HELLO_HANDLER_HPP_

#include <proxygen/httpserver/RequestHandler.h>

namespace ggml { namespace handler {

class HelloHandler : public proxygen::RequestHandler {
public:
    HelloHandler();

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;
};

} // namespace handler
} // namespace ggml

#endif // SRC_HELLO_HANDLER_HPP_
