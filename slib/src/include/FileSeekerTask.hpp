#ifndef FILE_SEEKER_TASK
#define FILE_SEEKER_TASK

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

namespace fs = std::filesystem;

class FileSeekerTask
{
    std::string filename;
    std::string directory;
    FileSeekersPool &pool;

    bool stop;
    std::set<fs::path> visited;
    std::stack<fs::path> stack;

    void searchDirs();

    void searchDir(const fs::path &dir);

    void checkEntry(const fs::directory_entry &entry);

    void checkFile(const fs::directory_entry &entry);

public:
    FileSeekerTask(const std::string &file, const fs::path &dir, FileSeekersPool &pool);
    void operator()();
};

#endif