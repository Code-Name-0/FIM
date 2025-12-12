cmake_minimum_required(VERSION 3.15)

#Metadata
project(FileIntegrityMonitor
	VERSION 1.0.0
	LANGUAGES CXX
)

# using cpp 17 for std::filesystem

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Deps : find OpenSSL
find_package(OpenSSL REQUIRED)


# executable

add_executable( fim 
				src/main.cpp
				src/Hasher.cpp)

# link deps

target_link_libraries(fim PRIVATE OpenSSL::Crypto)

message(STATUS "OpenSSL Found ${OPENSSL_VERSION}")