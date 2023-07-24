#include "include/FileSeeker.hpp"
#include "include/FileSeekerTask.hpp"
#include "include/FileSeekersPool.hpp"

void FileSeeker::findFile(const fs::path &dir, const std::string &filename)
{
    std::cout << "Searching for '" << filename << "' in '" << dir << "'" << std::endl;

    std::vector<fs::path> rootDirs;

    for (const auto &entry : fs::directory_iterator(dir))
    {
        if (fs::is_directory(entry.status()))
        {   
            if (this->isSystemDir(entry))
            {
                continue;
            }

            rootDirs.push_back(entry);
        }
        else if (fs::is_regular_file(entry.status()))
        {
            if (entry.path().filename() == filename)
            {
                std::cout << "File found at: " << entry.path() << std::endl;
                return;
            }
        }
    }

    this->findFileConcurrenty(rootDirs, filename);
}

void FileSeeker::findFileConcurrenty(const std::vector<fs::path> &dirs, const std::string &filename)
{
    std::cout << "Searching for '" << filename << "' concurrently" << std::endl;

    {
        FileSeekersPool fileSeekersPool{8};
        fileSeekersPool.process(dirs, filename);
        fileSeekersPool.join();
    }
}

bool FileSeeker::isSystemDir(const fs::directory_entry &entry)
{
    int count = this->systemDirs.count(entry.path());

    return count;
}