#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <string_view>

#define NOT_COPYABLE(TypeName)           \
TypeName(const TypeName&)=delete;        \
TypeName& operator=(TypeName&) = delete;

#define NOT_MOVEABLE(TypeName)            \
TypeName(const TypeName&&)=delete;        \
TypeName& operator=(TypeName&&) = delete;

#define NOT_COPYABLE_AND_MOVEABLE(TypeName) \
NOT_COPYABLE(TypeName)                      \
NOT_MOVEABLE(TypeName)

namespace tsrpp
{
static constexpr std::string_view salt{"damiantomczak"};

inline std::string hashPassword(const std::string& password)
{
    EVP_MD_CTX* mdctx{};
    const EVP_MD* md{};
    int errorCode;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr)
    {
        throw std::runtime_error{"EVP_MD_CTX_new couldn't create the context"};
    }

    std::string passwordWithSalt{password};
    passwordWithSalt += salt;

    // errorCode = EVP_DigestInit_ex(mdctx, md, nullptr);
    // errorCode = EVP_DigestUpdate(mdctx, passwordWithSalt.data(), passwordWithSalt.length());
    // errorCode = EVP_DigestFinal_ex(mdctx, result.data(), reinterpret_cast<unsigned int*>(result.length()));

    std::string result(32, '\0');
    unsigned int length = result.length();
    errorCode = EVP_DigestInit_ex(mdctx, md, nullptr);
    std::cout << errorCode << std::endl;
    errorCode = EVP_DigestUpdate(mdctx, passwordWithSalt.data(), passwordWithSalt.length());
    std::cout << errorCode << std::endl;
    errorCode = EVP_DigestFinal_ex(mdctx, reinterpret_cast<unsigned char*>(result.data()), &length);
std::cout << errorCode << std::endl;
    EVP_MD_CTX_free(mdctx);

    if (errorCode != 1)
    {
        throw std::runtime_error{"Hashing password returned error"};
    }
    std::cout << result << std::endl;
    return result;
}

inline bool verifyPassword(const std::string& password, const std::string& hash)
{
    std::string passwordHash = hashPassword(password);
    return passwordHash == hash;
}

// TODO:: Temporary function untill ssl will be provided
inline std::string createUrl(const std::string& path)
{
    return "http://" + std::string{DOMAIN_NAME} + path;
}
}