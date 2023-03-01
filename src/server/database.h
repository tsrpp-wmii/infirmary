#include "tools.hpp"

#include "SQLiteCpp/SQLiteCpp.h"
#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/ostream.h"

#include <iostream>
#include <stdexcept>
#include <functional>

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
    void execute(std::function<void(void)> method);

    std::unique_ptr<SQLite::Database> mDatabase;
};
}