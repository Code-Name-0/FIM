#include <iostream>
#include <openssl/crypto.h>
#include <filesystem>
#include "../include/Hasher.hpp"

int main()
{
    try
    {
        Hasher hasher;

        std::filesystem::path target_file = "../src/main.cpp";
        std::cout << "Target: " << target_file << std::endl;

        std::string hash = hasher.calculate_hash(target_file);

        std::cout << "SHA-256 hash: " << hash << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}