#include "stdinc.h"
#include "plugin.h"
#include "vfs.h"
#include "eu4.h"

using namespace std;
using namespace experimental;

void VFSManager::EnumerateOurFiles()
{
    string gameroot = CSingleton<CPlugin>::Instance().GetGameDirectory().string();
    string ourroot = CSingleton<CPlugin>::Instance().GetVFSDirectory().string();

    string vfspath;
    string ourvfspath;
    string full_path;

    filesystem::recursive_directory_iterator recur_it(ourroot);

    while (recur_it != filesystem::recursive_directory_iterator())
    {
        if (filesystem::is_regular_file(recur_it->path()))
        {
            full_path = recur_it->path().string();

            vfspath = full_path.data() + ourroot.length() + 1;
            ourvfspath = full_path.data() + gameroot.length() + 1;

            files.emplace(vfspath, ourvfspath);
        }

        ++recur_it;
    }
}

void VFSManager::InitAndPatch()
{
    EnumerateOurFiles();
}
