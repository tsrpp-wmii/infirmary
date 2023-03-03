#include "database.h"

namespace SQLite
{
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    std::cerr << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
    std::abort();
}
}

namespace tsrpp
{
bool Database::addUser(const User& user)
{
    SQLite::Statement query{*mDatabase, "INSERT INTO users(pesel, password, first_name, last_name, email, active)"
        "VALUES (:pesel, :password, :first_name, :last_name, :email, 0)"};

    query.bind(":pesel", user.pesel);
    query.bind(":password", user.password);
    query.bind(":first_name", user.first_name);
    query.bind(":last_name", user.last_name);
    query.bind(":email", user.email);

    if (query.exec() == 1)
    {
        return true;
    }

    return false;
}


std::optional<Database::User> Database::getUser(const std::string& pesel)
{
    std::optional<Database::User> result;
    SQLite::Statement query{*mDatabase,
        "SELECT * FROM users WHERE pesel=:pesel LIMIT 1"};
    query.bind(":pesel", pesel);

    if (query.executeStep())
    {
        result = std::optional{User{
            .id = query.getColumn("id"),
            .pesel = query.getColumn("pesel"),
            .password = query.getColumn("password"),
            .first_name = query.getColumn("first_name"),
            .last_name = query.getColumn("last_name"),
            .email = query.getColumn("email"),
            .active = query.getColumn("active")
        }};
    }

    return result;
}
}