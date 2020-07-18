#include "DumpHandler.hpp"

#include <folly/portability/GFlags.h>
#include <folly/executors/CPUThreadPoolExecutor.h>
#include <proxygen/httpserver/HTTPServer.h>

DEFINE_int32(port, 8080, "Which port to bind for incoming HTTP(S) traffic.");
DEFINE_string(dumpPath, "", "Root path for saving incoming crash dumps.");
DEFINE_int32(numThreads, 2, "Number of threads to run on the server.");

int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    auto diskIOThreadPool = std::make_shared<folly::CPUThreadPoolExecutor>(
            FLAGS_numThreads, std::make_shared<folly::NamedThreadFactory>("StaticDiskIOThread"));
    folly::setCPUExecutor(diskIOThreadPool);

    std::vector<proxygen::HTTPServer::IPConfig> IPs = {
        { folly::SocketAddress("0.0.0.0", FLAGS_port, true), proxygen::HTTPServer::Protocol::HTTP }
    };

    proxygen::HTTPServerOptions options;
    options.threads = FLAGS_numThreads;
    options.idleTimeout = std::chrono::milliseconds(60000);
    options.shutdownOn = {SIGINT, SIGTERM};
    options.enableContentCompression = true;
    options.handlerFactories = proxygen::RequestHandlerChain().addThen<garg::handler::DumpHandlerFactory>(
            FLAGS_dumpPath).build();
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
