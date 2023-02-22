#include "server.h"

#include "drogon/drogon.h"
#include "fmt/core.h"

// TODO
#define IP "127.0.0.1"
#define PORT 80

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
    fmt::print("Server::{} built at {} started on http://{}:{} !\n", PROJECT_NAME, "[TODO]", IP, PORT);
    auto& app = drogon::app();
    app.addListener(IP, PORT);
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
