#ifndef FILE_SEEKER
#define FILE_SEEKER

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <atomic>
#include <functional>

#include "FileSeekersPool.hpp"

namespace fs = std::filesystem;

class FileSeeker
{
    std::set<std::string> systemDirs = {"/proc", "/sys", "/dev"};
    
    bool isSystemDir(const fs::directory_entry& entry);
    void findFileConcurrenty(const std::vector<fs::path> &rootDirs, const std::string &filename);
public:
    void findFile(const fs::path &dir, const std::string &filename);
};

#endif