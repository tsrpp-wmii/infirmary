#include "database.h"

namespace SQLite
{
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    assert(false && (std::string{apFile} + ":" + std::to_string(apLine) + ":" + " error: assertion failed (" + apExpr + ") in " + apFunc + "() with message \"" + apMsg + "\"\n").c_str());
}
}

namespace tsrpp
{
bool Database::addUser(const User& user)
{
    SQLite::Statement q{*mpDatabase, "INSERT INTO users(pesel, password, first_name, last_name, email, note, role)"
        "VALUES (:pesel, :password, :first_name, :last_name, :email, :note, 0)"};

    q.bind(":pesel", user.pesel);
    q.bind(":password", user.password);
    q.bind(":first_name", user.first_name);
    q.bind(":last_name", user.last_name);
    q.bind(":email", user.email);
    q.bind(":note", user.note);

    if (q.exec() == 1)
    {
        return true;
    }

    return false;
}


std::optional<Database::User> Database::getUser(const std::string& pesel)
{
    std::optional<Database::User> result;
    SQLite::Statement q{*mpDatabase, "SELECT * FROM users WHERE pesel=:pesel LIMIT 1"};

    q.bind(":pesel", pesel);

    if (q.executeStep())
    {
        result = std::optional{User{
            .id{q.getColumn("id")},
            .pesel{q.getColumn("pesel").getString()},
            .password{q.getColumn("password").getString()},
            .first_name{q.getColumn("first_name").getString()},
            .last_name{q.getColumn("last_name").getString()},
            .email{q.getColumn("email").getString()},
            .note{q.getColumn("note").getString()},
            .role{q.getColumn("role")}
        }};
    }

    return result;
}
}