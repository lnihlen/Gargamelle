#ifndef SRC_HANDLER_FACTORY_HPP_
#define SRC_HANDLER_FACTORY_HPP_

#include <proxygen/httpserver/RequestHandlerFactory.h>

namespace ggml { namespace handler {

class HandlerFactory : public proxygen::RequestHandlerFactory {
public:
    // apiPrefix is typically "/api/" and needs to include that final slash or we won't parse URLs correctly.
    HandlerFactory(const std::string& apiPrefix, const std::string& dumpPath, const std::string& symbolPath);
    void onServerStart(folly::EventBase*) noexcept override;
    void onServerStop() noexcept override;
    proxygen::RequestHandler* onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept override;

private:
    std::string m_apiPrefix;
    std::string m_dumpPath;
    std::string m_symbolPath;
};

} // namespace handler
} // namespace ggml

#endif // SRC_HANDLER_FACTORY_HPP_
