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

    return "";
}