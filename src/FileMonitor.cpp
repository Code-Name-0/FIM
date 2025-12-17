#include "../include/FileMonitor.hpp"

#include <stdexcept>
#include <iostream>

void FileMonitor::addWatchPath(const std::filesystem::path &path)
{

    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("directory not found : " + path.string());
    }

    if (!std::filesystem::is_directory(path))
    {
        throw std::runtime_error("not a directory : " + path.string());
    }

    watchPaths_.push_back(path);
}

void FileMonitor::scanDirectory(const std::filesystem::path &dir)
{

    for (const auto &entry : std::filesystem::recursive_directory_iterator(dir))
    {
        if (std::filesystem::is_regular_file(entry))
        {
            hashDB_.insert({entry.path().string(), hasher_.calculate_hash(entry.path())});
        }
    }
}

void FileMonitor::createBaseline()
{
    hashDB_.clear();
    for (const auto &path : watchPaths_)
    {
        scanDirectory(path);
    }
}

std::vector<FileChange> FileMonitor::checkIntegrity()
{

    std::unordered_map<std::string, ChangeType> currentState;

    std::vector<FileChange> changes;

    // * find changes and creations
    for (const auto &dirPath : watchPaths_)
    {

        for (const auto &entry : std::filesystem::recursive_directory_iterator(dirPath))
        {

            if (std::filesystem::is_regular_file(entry))
            {
                std::filesystem::path path = entry.path();

                if (hashDB_.find(path.string()) != hashDB_.end())
                {
                    std::string oldHash = hashDB_.at(path.string());

                    // ? the file is already in the DB
                    std::string newHash = hasher_.calculate_hash(path);

                    if (oldHash != newHash)
                    {
                        FileChange c;
                        c.newHash = newHash;
                        c.oldHash = oldHash;
                        c.path = path;
                        c.type = ChangeType::MODIFIED;
                        changes.push_back(c);
                        currentState.insert({path.string(), ChangeType::MODIFIED});
                    }
                    else
                    {
                        currentState.insert({path.string(), ChangeType::UNCHANGED});
                    }
                }
                else
                {
                    FileChange c;
                    c.newHash = hasher_.calculate_hash(path);
                    c.oldHash = "";
                    c.path = path;
                    c.type = ChangeType::ADDED;
                    changes.push_back(c);
                    currentState.insert({path.string(), ChangeType::ADDED});
                }
            }
        }
    }

    for (const auto &[path, oldHash] : hashDB_)
    {
        if (currentState.count(path) == 0)
        {
            FileChange c;
            c.newHash = "";
            c.oldHash = oldHash;
            c.path = path;
            c.type = ChangeType::DELETED;
            changes.push_back(c);
            currentState.insert({path, ChangeType::DELETED});
        }
    }

    return changes;
}