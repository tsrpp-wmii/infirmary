#pragma once

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <string_view>
#include <cstring>
#include <array>

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
static constexpr std::string_view salt{"tsrpp"};

inline std::string hashPassword(const std::string& password)
{
    std::string passwordWithSalt(password);
    passwordWithSalt += salt;

    const EVP_MD* md{};
    EVP_MD_CTX* context{};
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen;

    md = EVP_sha256();
    context = EVP_MD_CTX_new();

    EVP_DigestInit_ex(context, md, nullptr);
    EVP_DigestUpdate(context, password.c_str(), password.size());
    EVP_DigestFinal_ex(context, hash.data(), &hashLen);
    EVP_MD_CTX_free(context);

    std::string result(hashLen * 2, '\0');
    for (unsigned int i{}; i < hashLen; ++i)
        sprintf(&result[i*2], "%02x", hash[i]);

    return result;
}

inline bool verifyPassword(const std::string& password, const std::string& hash)
{
    return (hashPassword(password) == hash);
}

inline std::string createUrl(const std::string& path)
{
#ifdef _DEBUG
    return "http://" + std::string{DOMAIN_NAME} + path;
#else
    return "https://" + std::string{DOMAIN_NAME} + path;
#endif
}
}