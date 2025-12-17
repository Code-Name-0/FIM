#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Hasher.hpp"

enum class ChangeType
{
    ADDED,
    MODIFIED,
    DELETED,
    UNCHANGED
};

struct FileChange
{
    std::filesystem::path path;
    ChangeType type;
    std::string oldHash;
    std::string newHash;
};

class FileMonitor
{
public:
    // * add dir to watch
    void addWatchPath(const std::filesystem::path &path);

    // * creates the initial "safe" state
    void createBaseline();

    // * compare current state against the baseline and return the changes
    std::vector<FileChange> checkIntegrity();

private:
    std::vector<std::filesystem::path> watchPaths_;
    std::unordered_map<std::string, std::string> hashDB_; // path -> hash
    Hasher hasher_;

    // * recurse scan of one directory
    void scanDirectory(const std::filesystem::path &dir);
};