#pragma once

#include <iostream>

#include "tools.hpp"

#include <memory>

namespace SQLite
{
    class Database;
}

namespace tsrpp
{
class Server final
{
    NOT_COPYABLE_AND_MOVEABLE(Server);
public:
    Server();
    ~Server();

    void run();
private:
    std::unique_ptr<SQLite::Database> mDatabase;
};
}