#include "login_system.h"

LoginController::LoginStatus LoginController::postLogin(const drogon::HttpRequestPtr& pReq)
{
    tsrpp::Database database{SQLite::OPEN_READWRITE};

    auto pesel{pReq->getOptionalParameter<std::string>("pesel")};
    if ((!pesel) || (pesel->length() == 0))
    {
        return LoginStatus::INCORRECT_PESEL;
    }

    auto password{pReq->getOptionalParameter<std::string>("password")};
    if ((!password) || (password->length() == 0))
    {
        return LoginStatus::INCORRECT_PASSWORD;
    }

    auto user{database.getUser(*pesel)};
    if (user)
    {
        if (tsrpp::verifyPassword(*password, user->password))
        {
            return LoginStatus::SUCCESS;
        }

    }

    return LoginStatus::FAILURE;
}

RegisterController::RegistrationStatus RegisterController::postRegister(const drogon::HttpRequestPtr& pReq)
{
    tsrpp::Database database{SQLite::OPEN_READWRITE};

    auto firstName{pReq->getOptionalParameter<std::string>("firstName")};
    if ((!firstName) || (firstName->length() == 0))
    {
        return RegistrationStatus::INCORRECT_FIRST_NAME;
    }

    auto lastName{pReq->getOptionalParameter<std::string>("lastName")};
    if ((!lastName) || (lastName->length() == 0))
    {
        return RegistrationStatus::INCORRECT_LAST_NAME;
    }

    auto pesel{pReq->getOptionalParameter<std::string>("pesel")};
    if ((!pesel) || (pesel->length() == 0))
    {
        return RegistrationStatus::INCORRECT_PESEL;
    }

    auto email{pReq->getOptionalParameter<std::string>("email")};
    if ((!email) || (email->length() == 0))
    {
        return RegistrationStatus::INCORRECT_EMAIL;
    }

    auto password{pReq->getOptionalParameter<std::string>("password")};
    if ((!password) || (password->length() == 0))
    {
        return RegistrationStatus::INCORRECT_PASSWORD;
    }

    auto hasUser{database.getUser(pesel.value())};
    if (hasUser)
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