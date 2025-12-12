#include <iostream>
#include <openssl/crypto.h>

int main()
{
    std::cout << "--- Phase 1       ---" << std::endl;
    std::cout << "OpenSSL version: " << OpenSSL_version(OPENSSL_VERSION) << std::endl;

    return 0;
}