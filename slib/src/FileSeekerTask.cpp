#include "include/FileSeeker.hpp"
#include "include/FileSeekerTask.hpp"
#include "include/FileSeekersPool.hpp"

FileSeekerTask::FileSeekerTask(const std::string &file, const fs::path &dir, FileSeekersPool &seekersPool)
    : filename(file), directory(dir), pool(seekersPool), stop{false}
{
    this->stack.push(directory);
}

void FileSeekerTask::operator()()
{
    try
    {   
        std::cout << "Searching for " << this->filename << " in " << this->directory << std::endl;
        this->searchDirs();
    }
    catch (const fs::filesystem_error &ex)
    {
        std::cerr << "Error while searching for the file: " << ex.what() << std::endl;
    }
}

void FileSeekerTask::searchDirs()
{
    while (!this->stack.empty() && !this->stop)
    {
        fs::path dir = this->stack.top();
        this->stack.pop();

        auto it = this->visited.find(dir);
        if (it == this->visited.end())
        {
            this->searchDir(dir);
        }
    }
}

void FileSeekerTask::searchDir(const fs::path &dir)
{

    try
    {
        for (const auto &entry : fs::directory_iterator(dir))
        {
            this->checkEntry(entry);
        }
        this->visited.insert(dir);
    }
    catch (const fs::filesystem_error &ex)
    {
        // permission denied
    }
}

void FileSeekerTask::checkEntry(const fs::directory_entry &entry)
{
    if (fs::is_symlink(entry))
    {
        fs::path targetPath = fs::read_symlink(entry.path());

        fs::directory_entry targetEntry(targetPath);

        if (fs::is_directory(targetEntry.status()))
        {
            this->stack.push(targetEntry.path());
        }
    }
    if (fs::is_directory(entry.status()))
    {
        this->stack.push(entry.path());
    }
    else if (fs::is_regular_file(entry.status()))
    {
        this->checkFile(entry);
    }
}

void FileSeekerTask::checkFile(const fs::directory_entry &entry)
{   
    if (entry.path().filename() == filename)
    {
        std::cout << "\033[31m" << "Found " << filename << " in " << entry << "\033[0m" << std::endl;
        this->pool.setFoundFile();
        this->stop = true;
    }
}