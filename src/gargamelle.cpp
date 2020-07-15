#include "spdlog/spdlog.h"
#include "Wt/WResource.h"
#include "Wt/WServer.h"
#include "Wt/Http/Request.h"
#include "Wt/Http/Response.h"

class DumpUploadResource : public Wt::WResource {
public:
    virtual void handleRequest(const Wt::Http::Request& request, Wt::Http::Response& response) override {
        spdlog::info("got something!");
    }
};

int main(int argc, char* argv[]) {
    DumpUploadResource dumpUpload;

    try {
        spdlog::info("Starting Server");
        Wt::WServer server{argc, argv, WTHTTP_CONFIGURATION};

        server.addResource(&dumpUpload, "/dump");
        server.run();
    } catch (const Wt::WServer::Exception& e) {
        spdlog::critical("Wt exception: {}", e.what());
        return -1;
    } catch (const std::exception& e) {
        spdlog::critical("C++ exception: {}", e.what());
        return -1;
    }

    return 0;
}
