#pragma once
#include <string>
#include <filesystem>
#include <openssl/evp.h>

class Hasher
{
public:
    Hasher();
    ~Hasher();

    std::string calculate_hash(const std::filesystem::path &filePath);

    // ! I do not understand this
    // * basically this is saying you cannot copy a Hasher object
    // * or use the = operator to set one object equale to the other
    // * this is critical to prevent double free problem.
    Hasher(const Hasher &) = delete;
    Hasher &operator=(const Hasher &) = delete;

private:
    EVP_MD_CTX *context;
};