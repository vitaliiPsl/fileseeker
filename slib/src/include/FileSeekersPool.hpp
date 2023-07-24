#ifndef FILE_SEEKERS_POOL
#define FILE_SEEKERS_POOL

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

class FileSeekersPool
{
    size_t numThreads;
    std::atomic<bool> jobsDone{false};
    std::atomic<bool> fileFound{false};
    std::mutex mutex;
    std::condition_variable condition;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

    void workerLoop();
    void startWorkers();
    void addJob(const std::function<void()> &job);
public:
    FileSeekersPool(size_t numThreads = 8);
    ~FileSeekersPool();
    void process(const std::vector<fs::path> rootDirs, const std::string &filename);
    void setFoundFile();
    void join();
};

#endif