#include "server.h"

#include "drogon/drogon.h"
#include "fmt/core.h"
#include "SQLiteCpp/SQLiteCpp.h"

// TODO: Handle this at the level of cmake
#define IP "127.0.0.1"
#define PORT 80

namespace tsrpp
{
Server::Server()
{
    fmt::print("SQlite3 version {} ({})\n", SQLite::VERSION, SQLite::getLibVersion());
    fmt::print("SQliteC++ version {}\n", SQLITECPP_VERSION);
}

Server::~Server()
{

}

void Server::run()
{
    // TODO: Compilation date and time should be printed below
    fmt::print("Server::{} built at {} started on http://{}:{} !\n", PROJECT_NAME, "[TODO]", IP, PORT);
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr&,
           std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("Hello, World!");
            callback(resp);
        },
        {drogon::Get});

    drogon::app().addListener(IP, PORT).run();
}
}
