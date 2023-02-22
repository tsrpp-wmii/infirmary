#include "server.h"

#include "drogon/drogon.h"

namespace tsrpp
{
Server::Server()
{
}

Server::~Server()
{

}

void Server::run()
{
    std::cout << "Server::" PROJECT_NAME " started!\n";
    auto& app = drogon::app();
    app.addListener("127.0.0.1", 80);
    app.setDocumentRoot("html");
    app.registerHandler(
        "/",
        [](const drogon::HttpRequestPtr &,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("Hello, World!");
            callback(resp);
        },
        {drogon::Get});
    app.run();
}
}
