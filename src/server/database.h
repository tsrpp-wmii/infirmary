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
// TODO: use GLFS for database.db
class Database final
{
public:
    Database(const int flags = SQLite::OPEN_READONLY) :
        mpDatabase{std::make_unique<SQLite::Database>(DATABASE_PATH, flags)}
    {}
    ~Database() = default;

    struct User
    {
        enum class Role
        {
            INVALID = -1,
            PATIENT,
            DOCTOR,
            RECEPTIONIST
        };

        std::int32_t id;
        std::string pesel;
        std::string password;
        std::string first_name;
        std::string last_name;
        std::string email;
        std::string note;
        Role role;
    };
    bool addUser(const User& user);
    std::optional<Database::User> getUser(const std::string& pesel);

private:
    std::unique_ptr<SQLite::Database> mpDatabase;
};
}