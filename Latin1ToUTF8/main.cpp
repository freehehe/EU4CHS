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

bool ConvertFile(const filesystem::path &in_file, const filesystem::path &out_file)
{
    static std::vector<char> cbuffer;
    static std::vector<uint32_t> wbuffer;

    fstream iofs{ in_file, ios::in };

    if (!iofs)
    {
        cout << "Something is wrong. Stopped." << std::endl;
        return false;
    }

    cbuffer.clear();
    wbuffer.clear();

    cbuffer.assign(istreambuf_iterator<char>{ iofs }, istreambuf_iterator<char>{});

    if (utf8::is_valid(cbuffer.begin(), cbuffer.end()))
    {
        return true;
    }

    transform(cbuffer.begin(), cbuffer.end(), back_inserter(wbuffer), 
        [](char character)
    {
        return *(unsigned char *)&character;
    });

    iofs.close();

    filesystem::create_directories(out_file.parent_path());

    iofs.open(out_file, ios::out | ios::trunc);

    if (!iofs)
    {
        cout << "Something is wrong. Stopped." << std::endl;
        return false;
    }

    utf8::utf32to8(wbuffer.begin(), wbuffer.end(), ostreambuf_iterator<char>{iofs});
    iofs.close();

    return true;
}

void ConvertDirectory(const filesystem::path &in_folder, const filesystem::path &out_folder)
{
    filesystem::recursive_directory_iterator dirit{ in_folder };

    while (dirit != filesystem::recursive_directory_iterator{})
    {
        auto _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".txt")
        {
            wstring pathstr = _path.wstring();
            pathstr.erase(0, in_folder.wstring().length());

            if (!ConvertFile(_path, out_folder / pathstr))
            {
                return;
            }
        }

        ++dirit;
    }
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        ConvertDirectory(argv[1], argv[2]);
    }
    else
    {
        ConvertDirectory("D:/Steam/steamapps/common/Europa Universalis IV", "C:/u8text");
    }

    return 0;
}
