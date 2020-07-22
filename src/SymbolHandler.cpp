#include "SymbolHandler.hpp"

#include <proxygen/httpserver/ResponseBuilder.h>

namespace ggml { namespace handler {

SymbolHandler::SymbolHandler(const std::string& symbolPath): m_symbolPath(symbolPath) {}

void SymbolHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept {

}

void SymbolHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
}

void SymbolHandler::onEOM() noexcept {
    proxygen::ResponseBuilder(downstream_).status(200, "OK").body("Symbol!").sendWithEOM();
}

void SymbolHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
}

void SymbolHandler::requestComplete() noexcept {
    delete this;
}

void SymbolHandler::onError(proxygen::ProxygenError err) noexcept {
    LOG(ERROR) << "symbolhandler got error";
    delete this;
}

} // namespace handler
} // namespace ggml
