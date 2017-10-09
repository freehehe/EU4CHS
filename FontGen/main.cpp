#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <cstdio>
#include <iterator>
#include <iostream>
#include <filesystem>
#include "../include/utf8cpp/utf8.h"

using namespace std;
using namespace std::experimental;

static set<uint32_t> collection;
static vector<uint32_t> wbuffer;
static vector<vector<uint32_t>> char_matrix(1);

void enumerate_chars(const filesystem::path &folder)
{
    ifstream ifs;

    filesystem::remove(folder / "charchart.txt");

    filesystem::recursive_directory_iterator dirit{ folder };

    while (dirit != filesystem::recursive_directory_iterator{})
    {
        auto _path = dirit->path();

        if (filesystem::is_regular_file(_path) && (_path.extension() == ".yml" || _path.extension() == ".txt"))
        {
            ifs.open(_path);

            if (!ifs)
            {
                continue;
            }

            wbuffer.clear();

            utf8::utf8to32(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>(), back_inserter(wbuffer));

            for (auto chr : wbuffer)
            {
                if (chr > 255 && chr != 0xFEFF)
                {
                    collection.insert(chr);
                }
            }

            ifs.close();
        }

        ++dirit;
    }
}

void generate_table(const filesystem::path &folder)
{
    for (auto chr : collection)
    {
        if (char_matrix.back().size() == 128)
        {
            char_matrix.emplace_back();
        }

        char_matrix.back().emplace_back(chr);
    }

    wbuffer.clear();

    wbuffer.push_back(0xFEFF); //不然Bitmap font generator不认

    for (auto &line : char_matrix)
    {
        copy(line.begin(), line.end(), back_inserter(wbuffer));
    }

    ofstream ofs(folder / "charchart.txt", ios::trunc);

    if (!ofs)
    {
        return;
    }

    utf8::utf32to8(wbuffer.begin(), wbuffer.end(), ostreambuf_iterator<char>(ofs));

    ofs.close();
}

int main(int argc, char **argv)
{
    enumerate_chars("C:\\eu4text");
    generate_table("C:\\eu4text");

    return 0;
}
