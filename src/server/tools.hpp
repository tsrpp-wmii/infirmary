#pragma once

#include <openssl/sha.h>

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
inline std::string createHash(const std::string& password)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.length());
    SHA256_Final(hash, &sha256);
    std::string result(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
    return result;
}

inline bool validatePassword(const std::string& password, const std::string& hash)
{
    std::string passwordHash = createHash(password);
    return passwordHash == hash;
}

// TODO:: Temporary function untill ssl will be provided
inline std::string createUrl(const std::string& path)
{
    return "http://" + std::string{DOMAIN_NAME} + path;
}
}