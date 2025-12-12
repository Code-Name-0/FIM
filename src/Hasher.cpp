#include "../include/Hasher.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

Hasher::Hasher()
{
    context = EVP_MD_CTX_new();
    if (context == nullptr)
    {
        throw std::runtime_error("Failed to initialize openssl context");
    }
}

Hasher::~Hasher()
{
    EVP_MD_CTX_free(context);
}

std::string Hasher::calculate_hash(const std::filesystem::path &filepath)
{
    // file preparation

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("cannot open file: " + filepath.string());
    }

    // ? reseting the context
    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1)
    {
        throw std::runtime_error("OSSL Digest init failed");
    }

    // ? reading the file in 4KB chunks
    const size_t buffer_size = 4096;
    char buffer[buffer_size];

    while (file.good())
    {
        file.read(buffer, buffer_size);

        if (EVP_DigestUpdate(context, buffer, file.gcount()) != 1)
        {
            throw std::runtime_error("OSSL Digest update failed");
        }
    }

    unsigned char hash_out[EVP_MAX_MD_SIZE];
    unsigned int length_of_hash = 0;

    if (EVP_DigestFinal_ex(context, hash_out, &length_of_hash) != 1)
    {
        throw std::runtime_error("OSSL Diegest Final failed");
    }

    // ? convert to hex string
    std::stringstream ss;

    for (unsigned int i = 0; i < length_of_hash; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash_out[i];
    }

    return ss.str();
}