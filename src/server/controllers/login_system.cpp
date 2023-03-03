#include "tools.hpp"
#include "database.h"

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>


class Controller
{
protected:
    bool isAlreadyLogged(const drogon::HttpRequestPtr& req, drogon::HttpResponsePtr& resp)
    {
        bool loggedIn{req->session()->getOptional<bool>("loggedIn").value_or(false)};
        if (loggedIn)
        {
            resp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/panel"));
            return true;
        }
        return false;
    }
};

class LoginController final : public drogon::HttpSimpleController<LoginController>, public Controller
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/login");
    PATH_LIST_END

    enum class LoginStatus
    {
        DEFAULT,
        INCORRECT_PESEL,
        INCORRECT_PASSWORD,
        FAILURE,
        SUCCESS
    };

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr resp;
        try
        {
            if (isAlreadyLogged(req, resp))
            {
                callback(resp);
                return;
            }

            LoginStatus loginStatus{};
            if (req->method() == drogon::HttpMethod::Post)
            {
                loginStatus = postLogin(req);
                if (loginStatus == LoginStatus::SUCCESS)
                {
                    resp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/panel"));
                    req->session()->insert("loggedIn", true);
                    callback(resp);
                    return;
                }
            }

            drogon::HttpViewData data;
            data.insert("loginStatus", static_cast<int>(loginStatus));

            resp = drogon::HttpResponse::newHttpViewResponse("login", data);
            callback(resp);
        }
        catch(const std::exception& e)
        {
            // TODO: code duplication
            fmt::print(std::cerr,
                fmt::format(fmt::fg(fmt::color::red), "tsrpp::exception {}\n", e.what()));
            resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("Something went wrong...");
            callback(resp);
        }
    }

    LoginStatus postLogin(const drogon::HttpRequestPtr& req)
    {
        tsrpp::Database database{SQLite::OPEN_READWRITE};

        auto pesel{req->getOptionalParameter<std::string>("pesel")};
        if ((!pesel.has_value()) || (pesel->length() == 0))
        {
            return LoginStatus::INCORRECT_PESEL;
        }

        auto password{req->getOptionalParameter<std::string>("password")};
        if ((!password.has_value()) || (password->length() == 0))
        {
            return LoginStatus::INCORRECT_PASSWORD;
        }

        auto user{database.getUser(*pesel)};
        if (user.has_value())
        {
            if (tsrpp::verifyPassword(*password, user->password))
            {
                return LoginStatus::SUCCESS;
            }

        }

        return LoginStatus::FAILURE;
    }
};

class LogoutController final : public drogon::HttpSimpleController<LogoutController>, public Controller
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

class RegisterController : public drogon::HttpSimpleController<RegisterController>, public Controller
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/register");
    PATH_LIST_END

    enum class RegistrationStatus
    {
        DEFAULT,
        INCORRECT_PESEL,
        INCORRECT_FIRST_NAME,
        INCORRECT_LAST_NAME,
        INCORRECT_PASSWORD,
        INCORRECT_EMAIL,
        DIFFERENT_PASSWORDS,
        ALREADY_EXISTS,
        SUCCESS
    };

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& req,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr resp;
        try
        {
            if (isAlreadyLogged(req, resp))
            {
                callback(resp);
                return;
            }

            RegistrationStatus registrationStatus{};
            if (req->method() == drogon::HttpMethod::Post)
            {
                registrationStatus = postRegister(req);
            }

            drogon::HttpViewData data;
            data.insert("registrationStatus", static_cast<int>(registrationStatus));

            resp = drogon::HttpResponse::newHttpViewResponse("registration", data);
            callback(resp);
        }
        catch(const std::exception& e)
        {
            // TODO: code duplication
            fmt::print(std::cerr,
                fmt::format(fmt::fg(fmt::color::red), "tsrpp::exception {}\n", e.what()));
            resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody("Something went wrong...");
            callback(resp);
        }

    }

    RegistrationStatus postRegister(const drogon::HttpRequestPtr& req)
    {
        tsrpp::Database database{SQLite::OPEN_READWRITE};

        auto firstName{req->getOptionalParameter<std::string>("firstName")};
        if ((!firstName.has_value()) || (firstName->length() == 0))
        {
            return RegistrationStatus::INCORRECT_FIRST_NAME;
        }

        auto lastName{req->getOptionalParameter<std::string>("lastName")};
        if ((!lastName.has_value()) || (lastName->length() == 0))
        {
            return RegistrationStatus::INCORRECT_LAST_NAME;
        }

        auto pesel{req->getOptionalParameter<std::string>("pesel")};
        if ((!pesel.has_value()) || (pesel->length() == 0))
        {
            return RegistrationStatus::INCORRECT_PESEL;
        }

        auto email{req->getOptionalParameter<std::string>("email")};
        if ((!email.has_value()) || (email->length() == 0))
        {
            return RegistrationStatus::INCORRECT_EMAIL;
        }

        auto password{req->getOptionalParameter<std::string>("password")};
        if ((!password.has_value()) || (password->length() == 0))
        {
            return RegistrationStatus::INCORRECT_PASSWORD;
        }

        auto hasUser{database.getUser(pesel.value())};
        if (hasUser.has_value())
        {
            return RegistrationStatus::ALREADY_EXISTS;
        }

        auto repeatedPassword{req->getOptionalParameter<std::string>("repeatedPassword")};
        if (password != repeatedPassword)
        {
            return RegistrationStatus::DIFFERENT_PASSWORDS;
        }

        auto hashedPassword = tsrpp::hashPassword(*password);

        if (!database.addUser({
            .pesel{std::move(*pesel)},
            .password{std::move(hashedPassword)},
            .first_name{std::move(*firstName)},
            .last_name{std::move(*lastName)},
            .email{std::move(*email)}}))
        {
            throw std::runtime_error("couldn't add user");
        }

        return RegistrationStatus::SUCCESS;
    }
};