#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <algorithm>
#include "../include/utf8cpp/utf8.h"

using namespace std;
using namespace std::experimental;

bool ConvertFile(const filesystem::path &path)
{
    static std::vector<char> cbuffer;
    static std::vector<uint32_t> wbuffer;

    fstream iofs{ path, ios::in };

    if (!iofs)
    {
        cout << "Something is wrong. Stopped." << std::endl;
        return false;
    }

    cbuffer.clear();
    wbuffer.clear();

    copy(istreambuf_iterator<char>{iofs}, istreambuf_iterator<char>(), back_inserter(cbuffer));
    transform(cbuffer.begin(), cbuffer.end(), back_inserter(wbuffer), 
        [](char character)
    {
        return *(unsigned char *)&character;
    });

    iofs.close();

    iofs.open(path, ios::out | ios::trunc);

    if (!iofs)
    {
        cout << "Something is wrong. Stopped." << std::endl;
        return false;
    }

    utf8::unchecked::utf32to8(wbuffer.begin(), wbuffer.end(), ostreambuf_iterator<char>{iofs});
    iofs.close();

    return true;
}

void ConvertDirectory(const filesystem::path &path)
{
    filesystem::recursive_directory_iterator dirit{ path };

    while (dirit != filesystem::recursive_directory_iterator{})
    {
        auto _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".txt")
        {
            if (!ConvertFile(_path))
            {
                return;
            }
        }

        ++dirit;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        ConvertDirectory(argv[1]);
    }
    else
    {
        ConvertDirectory("C:/province_names/");
    }
    return 0;
}
