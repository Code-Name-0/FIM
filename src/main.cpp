#include <iostream>
#include <openssl/crypto.h>
#include <filesystem>
#include "../include/FileMonitor.hpp"

std::string changeTypeToString(ChangeType type)
{
    switch (type)
    {
    case ChangeType::ADDED:
        return "ADDED";
    case ChangeType::MODIFIED:
        return "MODIFIED";
    case ChangeType::DELETED:
        return "DELETED";
    case ChangeType::UNCHANGED:
        return "UNCHANGED";
    default:
        return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path1> [path2] [path3]..." << std::endl;
        return 1;
    }
    try
    {
        FileMonitor monitor;

        for (int i = 1; i < argc; i++)
        {
            monitor.addWatchPath(argv[i]);
        }

        monitor.createBaseline();
        std::cout << "Baseline created!" << std::endl;

        std::cout << "modify/add/delete files, then press Enter..." << std::endl;
        std::cin.get();

        std::vector<FileChange> changes = monitor.checkIntegrity();

        if (changes.empty())
        {
            std::cout << "No changes detected." << std::endl;
        }
        else
        {
            for (const auto &change : changes)
            {
                std::cout << "File: " << change.path << " : " << changeTypeToString(change.type) << std::endl;
                // Print the type of change
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}