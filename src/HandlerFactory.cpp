#include "HandlerFactory.hpp"

#include "DumpHandler.hpp"
#include "ErrorHandler.hpp"
#include "HelloHandler.hpp"
#include "Paths.hpp"
#include "SymbolHandler.hpp"

#include <string.h>

namespace ggml { namespace handler {

HandlerFactory::HandlerFactory(const std::string& apiPrefix, const std::string& dumpPath,
        const std::string& symbolPath):
    m_apiPrefix(apiPrefix),
    m_dumpPath(dumpPath),
    m_symbolPath(symbolPath) {}

void HandlerFactory::onServerStart(folly::EventBase*) noexcept {}

void HandlerFactory::onServerStop() noexcept {}

proxygen::RequestHandler* HandlerFactory::onRequest(proxygen::RequestHandler*,
        proxygen::HTTPMessage* request) noexcept {
    auto requestPath = request->getPathAsStringPiece();
    if (strncmp(m_apiPrefix.data(), requestPath.start(), m_apiPrefix.size()) != 0) {
        LOG(ERROR) << "request '" << requestPath.start() << "' didn't match prefix.";
        return new ErrorHandler(404, "Not Found");
    }

    const char* pathStart = requestPath.start() + m_apiPrefix.size();
    size_t pathLength;

    // Need to discard any query parameters at end of query for string match to work.
    const char* queryStart = strchr(pathStart, '?');
    if (queryStart) {
        pathLength = queryStart - pathStart;
    } else {
        pathLength = requestPath.size() - m_apiPrefix.size();
    }

    Paths path = getPath(pathStart, pathLength);

    switch (path) {
    case kDump:
        return new DumpHandler(m_dumpPath);

    case kHello:
        return new HelloHandler();

    case kNotFound:
        LOG(ERROR) << "request '" << requestPath.start() << "' not found.";
        return new ErrorHandler(404, "Not Found");

    case kSymbol:
        return new SymbolHandler(m_symbolPath);
    }

    LOG(ERROR) << "at bottom of HandlerFactory.";
    return new ErrorHandler(500, "Server Error");
}


} // namespace handler
} // namespace ggml
