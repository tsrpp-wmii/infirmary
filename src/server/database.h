#include "tools.hpp"

#include "SQLiteCpp/SQLiteCpp.h"
#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/ostream.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <optional>

namespace tsrpp
{
class Database final
{
    NOT_COPYABLE_AND_MOVEABLE(Database);

public:
    Database(const int flags = SQLite::OPEN_READONLY);
    ~Database() = default;

    struct User
    {
        std::int32_t id;
        std::string pesel;
        std::string password;
        std::string first_name;
        std::string last_name;
        std::string email;
        std::int32_t active;
    };
    bool addUser(User&& user);
    std::optional<User> getUser(const std::string& pesel);

private:
    bool execute(std::function<void(void)> method);

    std::unique_ptr<SQLite::Database> mDatabase;
};
}