#include "ErrorHandler.hpp"

#include <proxygen/httpserver/ResponseBuilder.h>

namespace ggml { namespace handler {

ErrorHandler::ErrorHandler(int code, const char* message): m_code(code), m_message(message) {}

void ErrorHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept {
}

void ErrorHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
}

void ErrorHandler::onEOM() noexcept {
    proxygen::ResponseBuilder(downstream_).status(m_code, m_message).body(m_message).sendWithEOM();
}

void ErrorHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
}

void ErrorHandler::requestComplete() noexcept {
    delete this;
}

void ErrorHandler::onError(proxygen::ProxygenError err) noexcept {
    delete this;
}

} // namespace handler
} // namespace ggml
