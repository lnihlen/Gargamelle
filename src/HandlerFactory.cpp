#include "HandlerFactory.hpp"

#include "DumpHandler.hpp"
#include "ErrorHandler.hpp"
#include "HelloHandler.hpp"
#include "Paths.hpp"

#include <string.h>

namespace ggml { namespace handler {

HandlerFactory::HandlerFactory(const std::string& apiPrefix, const std::string& dumpPath):
    m_apiPrefix(apiPrefix),
    m_dumpPath(dumpPath) {}

void HandlerFactory::onServerStart(folly::EventBase*) noexcept {}

void HandlerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* HandlerFactory::onRequest(proxygen::RequestHandler*,
        proxygen::HTTPMessage* request) noexcept {
    auto requestPath = request->getPathAsStringPiece();
    if (strncmp(m_apiPrefix.data(), requestPath.start(), m_apiPrefix.size()) != 0) {
        LOG(ERROR) << "request '" << requestPath.start() << "' didn't match prefix.";
        return new ErrorHandler(404, "Not Found");
    }
    Paths path = getPath(requestPath.start() + m_apiPrefix.size(), requestPath.size() - m_apiPrefix.size());
    switch (path) {
    case kDump:
        return new DumpHandler(m_dumpPath);

    case kHello:
        return new HelloHandler();

    case kNotFound:
        LOG(ERROR) << "request '" << requestPath.start() << "' not found.";
        return new ErrorHandler(404, "Not Found");
    }

    LOG(ERROR) << "at bottom of HandlerFactory.";
    return new ErrorHandler(500, "Server Error");
}


} // namespace handler
} // namespace ggml
