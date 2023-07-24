#include "include/FileSeeker.hpp"
#include "include/FileSeekerTask.hpp"
#include "include/FileSeekersPool.hpp"

FileSeekersPool::FileSeekersPool(size_t threads) : numThreads{threads}
{
    std::cout << "Starting workers pool with " << this->numThreads << " workers" << std::endl;
    for (size_t i = 0; i < numThreads; i++)
    {
        this->threads.emplace_back(std::thread(&FileSeekersPool::workerLoop, this));
    }
}

FileSeekersPool::~FileSeekersPool()
{
    this->join();
}

void FileSeekersPool::workerLoop()
{
    while (true)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            this->condition.wait(lock, [this]
                                 { return !this->jobs.empty() || this->fileFound || this->jobsDone; });

            if (this->fileFound)
            {
                return;
            }

            if (this->jobs.empty() && this->jobsDone)
            {
                return;
            }

            job = jobs.front();
            this->jobs.pop();
        }

        job();
    }
}

void FileSeekersPool::startWorkers()
{
}

void FileSeekersPool::addJob(const std::function<void()> &job)
{
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->jobs.push(job);
        this->condition.notify_one();
    }
}

void FileSeekersPool::process(const std::vector<fs::path> dirs, const std::string &filename)
{
    for (const auto &dir : dirs)
    {
        this->addJob(FileSeekerTask(filename, dir, *this));
    }
}

void FileSeekersPool::setFoundFile()
{
    this->fileFound = true;
    condition.notify_all();
}

void FileSeekersPool::join()
{
    this->jobsDone = true;
    condition.notify_all();

    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}
