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
Database::Database(const int flags) :
    mDatabase{nullptr}
{
    try
    {
        mDatabase = std::make_unique<SQLite::Database>(DATABASE_PATH, flags);
    }
    catch(const std::exception& exception)
    {
        fmt::print(std::cerr, fmt::format(fmt::fg(fmt::color::red), "tsrpp::Database::exception {}\n", exception.what()));
    }
}

bool Database::addUser(User&& user)
{
    std::unique_ptr<SQLite::Statement> query;
    auto method = [&]() {
        query = std::make_unique<SQLite::Statement>(*mDatabase, "INSERT INTO users(pesel, password, first_name, last_name, email, active)"
            "VALUES (:pesel, :password, :first_name, :last_name, :email, 0)");

        query->bind(":pesel", user.pesel);
        query->bind(":password", user.password);
        query->bind(":first_name", user.first_name);
        query->bind(":last_name", user.last_name);
        query->bind(":email", user.email);

        query->exec();
    };
    if (execute(method)) {
        return true;
    }
    return false;
}


std::optional<Database::User> Database::getUser(const std::string& pesel)
{
    std::optional<Database::User> result;
    auto method = [&]() {
        SQLite::Statement query{*mDatabase,
            "SELECT * FROM users WHERE pesel=:pesel LIMIT 1"};

        query.bind(":pesel", pesel);

        if (query.executeStep())
        {
            result = std::make_optional(User{
                .id = query.getColumn("id"),
                .pesel = query.getColumn("pesel"),
                .password = query.getColumn("password"),
                .first_name = query.getColumn("first_name"),
                .last_name = query.getColumn("last_name"),
                .email = query.getColumn("email"),
                .active = query.getColumn("active")
            });
        }
    };

    if (!execute(method))
    {
        return std::nullopt;
    }

    return result;
}

bool Database::execute(std::function<void(void)> method) {
    try  {
        method();
        return true;
    }
    catch(std::exception& exception)  {
        fmt::print(std::cerr, fmt::format(fmt::fg(fmt::color::red), "tsrpp::Database::exception {}\n", exception.what()));
        return false;
    }
} 
}