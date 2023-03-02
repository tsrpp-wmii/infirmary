#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <string_view>
#include <cstring>
#include <array>
#include <string>
#include <stdexcept>

#define NOT_COPYABLE(TypeName)           \
TypeName(const TypeName&)=delete;        \
TypeName& operator=(TypeName&) = delete;

#define NOT_MOVEABLE(TypeName)            \
TypeName(const TypeName&&)=delete;        \
TypeName& operator=(TypeName&&) = delete;

#define NOT_COPYABLE_AND_MOVEABLE(TypeName) \
NOT_COPYABLE(TypeName)                      \
NOT_MOVEABLE(TypeName)

#define XSTR(x) #x
#define STR(x) XSTR(x)

namespace tsrpp
{
inline constexpr std::string_view salt{"tsrpp"};

inline std::string hashPassword(const std::string& password)
{
    std::string passwordWithSalt(password);
    passwordWithSalt += salt;

    const EVP_MD* md{};
    EVP_MD_CTX* context{};
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen{};
    auto errorCode = 1;

    md = EVP_sha256();
    context = EVP_MD_CTX_new();
    if (md == nullptr || context == nullptr)
    {
        throw std::runtime_error{"OpenSSL couldn't create context"};
    }

    errorCode = EVP_DigestInit_ex(context, md, nullptr);
    errorCode = EVP_DigestUpdate(context, password.c_str(), password.size());
    errorCode = EVP_DigestFinal_ex(context, hash.data(), &hashLen);
    EVP_MD_CTX_free(context);

    if (errorCode != 1)
    {
        throw std::runtime_error{"OpenSSSl couldn't hashed the password"};
    }

    std::string result(hashLen * 2, '\0');
    for (unsigned int i{}; i < hashLen; ++i)
    {
        sprintf(&result[i*2], "%02x", hash[i]);
    }

    return result;
}

inline bool verifyPassword(const std::string& password, const std::string& hash)
{
    return (hashPassword(password) == hash);
}

inline std::string createUrl(const std::string& path)
{
#ifdef NDEBUG
    constexpr std::string_view prefix{"https://" DOMAIN_NAME};
#else
    constexpr std::string_view prefix{"http://" DOMAIN_NAME};
#endif
    return std::string{prefix} + path;
}
}