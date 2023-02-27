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
    catch(const std::exception& e)
    {
        std::cerr << "\033[0;34mtsrpp::Database\033[0m " << e.what() << '\n';
    }
}

void Database::addUser(
    const std::string& firstName,
    const std::string& lastName,
    const std::string& email,
    const std::string& pesel,
    const std::string& password
    )
{
    try
    {
        SQLite::Statement query{*mDatabase, "INSERT INTO users(pesel, password, first_name, last_name, email, active)"
            "VALUES (:pesel, :password, :first_name, :last_name, :email, 0)"};

        query.bind(":pesel", pesel);
        query.bind(":password", password);
        query.bind(":first_name", firstName);
        query.bind(":last_name", lastName);
        query.bind(":email", email);

        query.exec();
    }
    catch (std::exception& e)
    {
        std::cerr << "\033[0;34mtsrpp::Database\033[0m " << e.what() << '\n';
    }
}
}