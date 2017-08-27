#include "stdinc.h"
#include "plugin.h"
#include "vfs.h"
#include "eu4.h"

using namespace std;
using namespace experimental;

void VFSManager::EnumerateOurFiles()
{
    string gameroot = CSingleton<CPlugin>::Instance().GetGameDirectory().string();
    string ourroot = (CSingleton<CPlugin>::Instance().GetPluginDirectory() / "eu4chs/vfsroot").string();

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

            replace(vfspath.begin(), vfspath.end(), '\\', '/');
            replace(ourvfspath.begin(), ourvfspath.end(), '\\', '/');

            files.emplace(hash<string_view>()(vfspath), ourvfspath);
        }

        ++recur_it;
    }
}

void *VFSManager::VFSOpenFile_0x8D(const char *virtualpath)
{
    auto it = CSingleton<VFSManager>::Instance().files.find(hash<string_view>()(virtualpath));

    if (it != CSingleton<VFSManager>::Instance().files.end())
    {
        virtualpath = it->second.c_str();
    }

    return injector::cstd<void*(const char *)>::call(game_meta.pfPHYSFS_openRead, virtualpath);
}

void VFSManager::InitAndPatch()
{
    EnumerateOurFiles();
    injector::MakeCALL(game_meta.pfVFSOpenFile + 0x8D, VFSOpenFile_0x8D);
}
