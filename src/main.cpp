#include "FileSeeker.hpp"

int main(int argc, char const *argv[])
{
    std::string filename;
    fs::path dir;

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename> [directory]" << std::endl;
        return 1;
    }

    filename = argv[1];

    if (argc >= 3)
    {
        dir = fs::path(argv[2]);
    }
    else
    {
        dir = fs::current_path().root_path();
    }

    FileSeeker fileSeeker;
    fileSeeker.findFile(dir, filename);

    return 0;
}
