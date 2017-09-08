#include <iostream>
#include "byte_pattern.h"
#include <filesystem>

using namespace std;
using namespace std::experimental;

int main()
{
    vector<char> buffer;

    filesystem::path source_path{ LR"(E:\欧陆风云4 1.22.1 英文原版\欧陆风云4 1.22.1 英文原版\Europa Universalis IV\eu4.exe)" };

    auto filesize = filesystem::file_size(source_path);

    FILE *cFile = fopen(source_path.string().c_str(), "rb");

    if (!cFile)
    {
        return 0;
    }

    buffer.resize(filesize);
    fread(buffer.data(), filesize, 1, cFile);
    fclose(cFile);

    g_pattern.set_pattern("80 F? A?");

    g_pattern.set_range(buffer.data(), buffer.data() + filesize);

    g_pattern.force_search();

    auto size1 = g_pattern.size();

    cout << size1 << endl;

    return 0;
}
