#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

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
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("login");
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
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody("logout");
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