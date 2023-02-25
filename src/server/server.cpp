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
    fmt::print("SQlite3 version {} ({})\n", SQLite::VERSION, SQLite::getLibVersion());
    fmt::print("SQliteC++ version {}\n", SQLITECPP_VERSION);
}

Server::~Server()
{

}

void Server::run()
{
    // TODO: Compilation date and time should be printed below
    fmt::print("\nServer::{} built at {} started on \033[31mhttp://{}:{}\033[0m !\n",
        PROJECT_NAME,
        "[TODO]",
        IP,
        PORT
    );

    drogon::app()
        .setDocumentRoot(DOCUMENT_ROOT_PATH)
        .enableSession(24h)
        .addListener(IP, PORT)
        .run();
}
}
