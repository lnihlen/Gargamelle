#ifndef SRC_SYMBOL_HANDLER_HPP_
#define SRC_SYMBOL_HANDLER_HPP_

#include <proxygen/httpserver/RequestHandler.h>

namespace ggml { namespace handler {

class SymbolHandler : public proxygen::RequestHandler {
public:
    SymbolHandler(const std::string& symbolPath);

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept override;
    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(proxygen::ProxygenError err) noexcept override;
private:
    std::string m_symbolPath;
};

} // namespace handler
} // namespace ggml

#endif // SRC_SYMBOL_HANDLER_HPP_
