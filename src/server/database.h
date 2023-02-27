#include "tools.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

#include <iostream>

namespace tsrpp
{
class Database final
{
    NOT_COPYABLE_AND_MOVEABLE(Database);
public:
    Database(const int flags = SQLite::OPEN_READONLY);
    ~Database() = default;

    void addUser(
        const std::string& firstName,
        const std::string& lastName,
        const std::string& email,
        const std::string& pesel,
        const std::string& password
    );

private:
    std::unique_ptr<SQLite::Database> mDatabase;
};
}