#include "DumpHandler.hpp"

#include <folly/FileUtil.h>
#include <folly/Memory.h>
#include <folly/executors/GlobalExecutor.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/Unistd.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

namespace garg { namespace handler {

DumpHandler::DumpHandler(const std::string& dumpPath): m_valid(false), m_dumpPath(dumpPath), m_dumpFile(-1) {}

void DumpHandler::onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept {
    // POST requests only, and guid query parameter required
    auto method = request->getMethod();
    if (method != proxygen::HTTPMethod::POST || !request->hasQueryParam("guid")) {
        LOG(ERROR) << request->getURL() << " bad request.";
        proxygen::ResponseBuilder(downstream_).status(404, "Not Found").body("file not found").sendWithEOM();
        return;
    }

    m_guid = request->getQueryParam("guid");
    m_dumpFile = folly::openNoInt((m_dumpPath + m_guid + ".gz").data(),
            O_CREAT | O_TRUNC | O_RDWR);
    if (m_dumpFile < 0) {
        LOG(ERROR) << "failed to open file to write dump!";
        return;
    }
    m_valid = true;
}

void DumpHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
    if (m_body) {
        m_body->prependChain(std::move(body));
    } else {
        m_body = std::move(body);
    }
}

void DumpHandler::onEOM() noexcept {
    if (m_valid) {
        folly::getCPUExecutor()->add(std::bind(&DumpHandler::writeFile, this,
                    folly::EventBaseManager::get()->getEventBase()));
    } else {
        proxygen::ResponseBuilder(downstream_).status(400, "Not Found").sendWithEOM();
    }
}

void DumpHandler::onUpgrade(proxygen::UpgradeProtocol proto) noexcept {
}

void DumpHandler::requestComplete() noexcept {
    delete this;
}

void DumpHandler::onError(proxygen::ProxygenError err) noexcept {
    LOG(ERROR) << "dumphandler got error";
    delete this;
}

void DumpHandler::writeFile(folly::EventBase* evb) {
    folly::io::Cursor cursor(m_body.get());
    std::array<uint8_t, 4096> buf;
    size_t bytesPulled = cursor.pullAtMost(buf.data(), sizeof(buf));
    size_t bytesWritten = 0;
    while (bytesPulled > 0) {
        bytesWritten += folly::writeNoInt(m_dumpFile, buf.data(), bytesPulled);
        bytesPulled = cursor.pullAtMost(buf.data(), sizeof(buf));
    }

    LOG(INFO) << "wrote " << bytesWritten << " bytes to crash dump " << m_guid;
    evb->runInEventBaseThread([this] {
            proxygen::ResponseBuilder(downstream_).status(200, "OK").sendWithEOM();
    });
    close(m_dumpFile);
}

// ==== DumpHandlerFactory
DumpHandlerFactory::DumpHandlerFactory(const std::string& dumpPath): m_dumpPath(dumpPath) {}
void DumpHandlerFactory::onServerStart(folly::EventBase*) noexcept {}
void DumpHandlerFactory::onServerStop() noexcept {}
proxygen::RequestHandler* DumpHandlerFactory::onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept {
    return new DumpHandler(m_dumpPath);
}

} // namespace handler
} // namespace garg
