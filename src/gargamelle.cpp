// #include "spdlog/spdlog.h"

#include <folly/Memory.h>

#include <proxygen/httpserver/HTTPServer.h>

#include <proxygen/httpserver/ResponseBuilder.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#include <vector>

class DumpHandler : public proxygen::RequestHandler {
public:
    DumpHandler();

    void onRequest(std::unique_ptr<proxygen::HTTPMessage> request) noexcept override {
    }

    void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
        if (m_body) {
            m_body->prependChain(std::move(m_body));
        } else {
            m_body = std::move(body);
        }
    }

    void onEOM() noexcept override {
        proxygen::ResponseBuilder(downstream_).status(200, "OK").sendWithEOM();
    }

    void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override {
    }

    void requestComplete() noexcept override {
        delete this;
    }

    void onError(proxygen::ProxygenError err) noexcept override {
        delete this;
    }

private:
    std::unique_ptr<folly::IOBuf> m_body;
};

class DumpHandlerFactory : public proxygen::RequestHandlerFactory {
public:
    void onServerStart(folly::EventBase* /* evb */) noexcept override {
    }

    void onServerStop() noexcept override {
    }

    proxygen::RequestHandler* onRequest(proxygen::RequestHandler*, proxygen::HTTPMessage*) noexcept override {
        return new DumpHandler();
    }

private:
};

int main(int argc, char* argv[]) {
    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        { folly::SocketAddress("localhost", 8080, true), proxygen::HTTPServer::Protocol::HTTP }
    };

    proxygen::HTTPServerOptions options;
    options.threads = 4;
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = false;
    options.handlerFactories = proxygen::RequestHandlerChain().addThen<DumpHandlerFactory>().build();
    // ?
    // options.h2cEnabled = true;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    std::thread t([&] () {
        server.start();
    });

    t.join();
    return 0;
}
