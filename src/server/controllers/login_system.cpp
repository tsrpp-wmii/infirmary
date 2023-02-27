#include "tools.hpp"
#include "database.h"

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>


class LoginController : public drogon::HttpSimpleController<LoginController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/login");
    PATH_LIST_END

    enum class LoginStatus
    {
        
    };

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr resp;

        bool loginAttempt{};
        if (req->method() == drogon::HttpMethod::Post)
        {
            std::string pesel = req->getParameter("pesel");
            std::string password = req->getParameter("passwd");
            std::string hashedPassword = tsrpp::hashPassword("password123");

            if (pesel == "root" && tsrpp::verifyPassword(password, hashedPassword))
            {
                resp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/panel"));
                req->session()->insert("loggedIn", true);
                callback(resp);
                return;
            }
            else
            {
                loginAttempt = true;
            }
        }

        drogon::HttpViewData data;
        data.insert("loginAttempt", loginAttempt);

        resp = drogon::HttpResponse::newHttpViewResponse("login", data);
        callback(resp);
    }
};

class LogoutController : public drogon::HttpSimpleController<LogoutController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/logout");
    PATH_LIST_END

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        const auto redirectionUrl = tsrpp::createUrl("/");
        drogon::HttpResponsePtr resp = drogon::HttpResponse::newRedirectionResponse(redirectionUrl);
        req->session()->erase("loggedIn");
        callback(resp);
    }
};

class RegisterController : public drogon::HttpSimpleController<RegisterController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/register");
    PATH_LIST_END

    enum class RegistrationStatus
    {
        DEFAULT,
        INCORRECT_PASSWORD,
        DIFFERENT_PASSWORDS,
        INCORRECT_MAIL,
        INCORRECT_PESEL,
        SUCCESS
    };

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr resp;

        RegistrationStatus registrationStatus{};
        if (req->method() == drogon::HttpMethod::Post)
        {
            std::string firstName = req->getParameter("firstName");
            std::string lastName = req->getParameter("lastName");
            std::string email = req->getParameter("email");
            std::string pesel = req->getParameter("pesel");
            std::string password = req->getParameter("password");
            std::string repeatedPassword = req->getParameter("repeatedPassword");

            // TODO: other registration's failures should be handled
            if (password != repeatedPassword)
            {
                registrationStatus = RegistrationStatus::DIFFERENT_PASSWORDS;
            }

            std::string hashedPassword = tsrpp::hashPassword(password);

            tsrpp::Database database{SQLite::OPEN_READWRITE};
            database.addUser(
                firstName,
                lastName,
                email,
                pesel,
                hashedPassword
            );

            registrationStatus = RegistrationStatus::SUCCESS;
        }

        drogon::HttpViewData data;
        data.insert("registrationStatus", static_cast<int>(registrationStatus));

        resp = drogon::HttpResponse::newHttpViewResponse("registration", data);
        callback(resp);
    }
};