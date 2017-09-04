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
                if (chr > 255 &&
                    chr != 0x152 &&
                    chr != 0x153 &&
                    chr != 0x160 &&
                    chr != 0x161 &&
                    chr != 0x178 &&
                    chr != 0x17d &&
                    chr != 0x17e &&
                    chr != 0x192 &&
                    chr != 0x2c6 &&
                    chr != 0x2dc &&
                    chr != 0x2013 &&
                    chr != 0x2014 &&
                    chr != 0x2018 &&
                    chr != 0x2019 &&
                    chr != 0x201a &&
                    chr != 0x201c &&
                    chr != 0x201d &&
                    chr != 0x201e &&
                    chr != 0x2020 &&
                    chr != 0x2021 &&
                    chr != 0x2022 &&
                    chr != 0x2026 &&
                    chr != 0x2030 &&
                    chr != 0x2039 &&
                    chr != 0x203a &&
                    chr != 0x20ac &&
                    chr != 0x2122 &&
                    chr != 0xfeff)
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
        if (char_matrix.back().size() == 64)
        {
            char_matrix.emplace_back();
        }

        char_matrix.back().emplace_back(chr);
    }

    wbuffer.clear();

    wbuffer.push_back(0xFEFF);

    for (auto &line : char_matrix)
    {
        copy(line.begin(), line.end(), back_inserter(wbuffer));
        wbuffer.push_back('\n');
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
    enumerate_chars("E:\\eu4text");
    generate_table("E:\\eu4text");

    return 0;
}
