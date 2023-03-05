#include "tools.hpp"
#include "database.h"

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>

class LoginSystemController
{
protected:
    bool isAlreadyLogged(const drogon::HttpRequestPtr& pReq, drogon::HttpResponsePtr& pResp)
    {
        bool loggedIn{pReq->session()->getOptional<bool>("loggedIn").value_or(false)};
        if (loggedIn)
        {
            pResp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/panel"));
            return true;
        }
        return false;
    }
};

class LoginController final : public drogon::HttpSimpleController<LoginController>, public LoginSystemController
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/login");
    PATH_LIST_END

    enum class LoginStatus
    {
        DEFAULT,
        REGISTRATION_SUCCESS,
        INCORRECT_PESEL,
        INCORRECT_PASSWORD,
        FAILURE,
        SUCCESS
    };

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& pReq,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr pResp;
        try
        {
            if (isAlreadyLogged(pReq, pResp))
            {
                callback(pResp);
                return;
            }

            LoginStatus loginStatus{};
            if (pReq->method() == drogon::HttpMethod::Post)
            {
                loginStatus = postLogin(pReq);
                if (loginStatus == LoginStatus::SUCCESS)
                {
                    pResp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/panel"));
                    pReq->session()->insert("loggedIn", true);
                    callback(pResp);
                    return;
                }
            }

            drogon::HttpViewData data;

            auto isRegistrationSuccess{pReq->getOptionalParameter<bool>("registrationSuccess").value_or(false)};
            if (isRegistrationSuccess)
            {
                loginStatus = LoginStatus::REGISTRATION_SUCCESS;
            }
            data.insert("loginStatus", static_cast<int>(loginStatus));

            pResp = drogon::HttpResponse::newHttpViewResponse("login", data);
            callback(pResp);
        }
        catch(const std::exception& e)
        {
            // TODO: code duplication
            fmt::print(std::cerr,
                fmt::format(fmt::fg(fmt::color::red), "tsrpp::exception {}\n", e.what()));
            pResp = drogon::HttpResponse::newHttpResponse();
            pResp->setBody("Something went wrong...");
            callback(pResp);
        }
    }

    LoginStatus postLogin(const drogon::HttpRequestPtr& pReq)
    {
        tsrpp::Database database{SQLite::OPEN_READWRITE};

        auto pesel{pReq->getOptionalParameter<std::string>("pesel")};
        if ((!pesel.has_value()) || (pesel->length() == 0))
        {
            return LoginStatus::INCORRECT_PESEL;
        }

        auto password{pReq->getOptionalParameter<std::string>("password")};
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

class LogoutController final : public drogon::HttpSimpleController<LogoutController>, public LoginSystemController
{
public:
    PATH_LIST_BEGIN
    PATH_ADD("/logout");
    PATH_LIST_END

    void asyncHandleHttpRequest(
        const drogon::HttpRequestPtr& pReq,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        const auto redirectionUrl = tsrpp::createUrl("/");
        drogon::HttpResponsePtr pResp = drogon::HttpResponse::newRedirectionResponse(redirectionUrl);
        pReq->session()->erase("loggedIn");
        callback(pResp);
    }
};

class RegisterController : public drogon::HttpSimpleController<RegisterController>, public LoginSystemController
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
        const drogon::HttpRequestPtr& pReq,
        std::function<void(const drogon::HttpResponsePtr&)>&& callback) override
    {
        drogon::HttpResponsePtr pResp;
        try
        {
            if (isAlreadyLogged(pReq, pResp))
            {
                callback(pResp);
                return;
            }

            RegistrationStatus registrationStatus{};
            if (pReq->method() == drogon::HttpMethod::Post)
            {
                registrationStatus = postRegister(pReq);
            }

            if (registrationStatus == RegistrationStatus::SUCCESS)
            {
                pResp = drogon::HttpResponse::newRedirectionResponse(tsrpp::createUrl("/login?registrationSuccess=true"));
                callback(pResp);
                return;
            }

            drogon::HttpViewData data;
            data.insert("registrationStatus", static_cast<int>(registrationStatus));

            pResp = drogon::HttpResponse::newHttpViewResponse("registration", data);
            callback(pResp);
        }
        catch(const std::exception& e)
        {
            // TODO: code duplication
            fmt::print(std::cerr,
                fmt::format(fmt::fg(fmt::color::red), "tsrpp::exception {}\n", e.what()));
            pResp = drogon::HttpResponse::newHttpResponse();
            pResp->setBody("Something went wrong...");
            callback(pResp);
        }

    }

    RegistrationStatus postRegister(const drogon::HttpRequestPtr& pReq)
    {
        tsrpp::Database database{SQLite::OPEN_READWRITE};

        auto firstName{pReq->getOptionalParameter<std::string>("firstName")};
        if ((!firstName.has_value()) || (firstName->length() == 0))
        {
            return RegistrationStatus::INCORRECT_FIRST_NAME;
        }

        auto lastName{pReq->getOptionalParameter<std::string>("lastName")};
        if ((!lastName.has_value()) || (lastName->length() == 0))
        {
            return RegistrationStatus::INCORRECT_LAST_NAME;
        }

        auto pesel{pReq->getOptionalParameter<std::string>("pesel")};
        if ((!pesel.has_value()) || (pesel->length() == 0))
        {
            return RegistrationStatus::INCORRECT_PESEL;
        }

        auto email{pReq->getOptionalParameter<std::string>("email")};
        if ((!email.has_value()) || (email->length() == 0))
        {
            return RegistrationStatus::INCORRECT_EMAIL;
        }

        auto password{pReq->getOptionalParameter<std::string>("password")};
        if ((!password.has_value()) || (password->length() == 0))
        {
            return RegistrationStatus::INCORRECT_PASSWORD;
        }

        auto hasUser{database.getUser(pesel.value())};
        if (hasUser.has_value())
        {
            return RegistrationStatus::ALREADY_EXISTS;
        }

        auto repeatedPassword{pReq->getOptionalParameter<std::string>("repeatedPassword")};
        if (password != repeatedPassword)
        {
            return RegistrationStatus::DIFFERENT_PASSWORDS;
        }

        auto hashedPassword = tsrpp::hashPassword(*password);

        auto note{pReq->getOptionalParameter<std::string>("note").value_or("")};

        if (!database.addUser({
            .pesel{std::move(*pesel)},
            .password{std::move(hashedPassword)},
            .first_name{std::move(*firstName)},
            .last_name{std::move(*lastName)},
            .email{std::move(*email)},
            .note{std::move(note)}}))
        {
            throw std::runtime_error("couldn't add user");
        }

        return RegistrationStatus::SUCCESS;
    }
};