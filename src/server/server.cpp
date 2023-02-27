#include "server.h"

#include "drogon/drogon.h"
#include "fmt/core.h"
#include "SQLiteCpp/SQLiteCpp.h"

using namespace std::chrono_literals;

// TODO: Handle this at the level of cmake
#define IP "127.0.0.1"
#define PORT 80

namespace tsrpp
{
Server::Server()
{
    fmt::print("SQlite3 version {} ({})\n", ::SQLite::VERSION, ::SQLite::getLibVersion());
    fmt::print("SQliteC++ version {}\n", SQLITECPP_VERSION);
}

void Server::run()
{
    // TODO: Compilation date and time should be printed below
    fmt::print("\n\nServer::{} built at {} started on \033[31mhttp://{}:{}\033[0m !\n\n",
        PROJECT_NAME,
        "[TODO]",
        IP,
        PORT
    );

    // TODO: it should be placed in json instead of hard-coded
    drogon::app()
        .setDocumentRoot(DOCUMENT_ROOT_PATH)
        .enableSession(24h)
        .setSSLFiles("", "") // TODO: fix ssl
        .addListener(IP, PORT)
        .addListener(IP, 443)
        .run();
}
}
