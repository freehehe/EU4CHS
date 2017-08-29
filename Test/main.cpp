#include <iostream>
#include "byte_pattern.h"
#include "Hooking.Patterns.h"

using namespace std;
using namespace std::experimental;

int main()
{
    std::vector<char> buffer;

    filesystem::path source_path{ "E:\\MaintenanceTool.exe" };

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

    hook::range_pattern pattern13((std::uintptr_t)buffer.data(), (std::uintptr_t)buffer.data() + filesize, "80 F? A?");

    g_pattern.set_range(buffer.data(), buffer.data() + filesize);

    auto size1 = g_pattern.force_search().size();
    auto size2 = pattern13.size();

    cout << size1 << endl << size2 << endl;

    return 0;
}
