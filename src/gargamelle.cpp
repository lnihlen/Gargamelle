#include "DumpHandler.hpp"

#include <folly/portability/GFlags.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <proxygen/httpserver/HTTPServer.h>

DEFINE_bool(useSSL, true, "Require SSL connections.");
DEFINE_int32(port, 440, "Which port to bind for incoming HTTP(S) traffic.");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    auto diskIOThreadPool = std::make_shared<folly::CPUThreadPoolExecutor>(
            4, std::make_shared<folly::NamedThreadFactory>("StaticDiskIOThread"));
    folly::setCPUExecutor(diskIOThreadPool);

    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        { folly::SocketAddress("0.0.0.0", FLAGS_port, true), proxygen::HTTPServer::Protocol::HTTP }
    };

    proxygen::HTTPServerOptions options;
    options.threads = 4;
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = true;
    options.handlerFactories = proxygen::RequestHandlerChain().addThen<garg::handler::DumpHandlerFactory>().build();
    options.h2cEnabled = false;

    proxygen::HTTPServer server(std::move(options));
    server.bind(IPs);

    std::thread t([&] () {
            LOG(INFO) << "starting server.";
        server.start();
    });

    t.join();
    return 0;
}
