#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

#include "tools.hpp"

class LoginController : public drogon::HttpSimpleController<LoginController>
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/login");
    PATH_LIST_END

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr resp;

        const auto method{req->method()};
        bool loginAttempt{};
        if (method == drogon::HttpMethod::Post)
        {
            std::string pesel = req->getParameter("pesel");
            std::string passwd = req->getParameter("passwd");
            std::string securePasswd = tsrpp::hashPassword("password123");

            if (pesel == "root" && tsrpp::verifyPassword(passwd, securePasswd))
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
        const auto redirectionUrl = tsrpp::createUrl("/login");
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

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("register");
        callback(resp);
    }
};