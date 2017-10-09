#include "vfs.h"
#include "eu4.h"
#include "plugin.h"

using namespace std;
using namespace std::experimental::filesystem::v1;

CVirtualPaths g_VirtualFiles;

void CVirtualPaths::EnumerateFolder(const path &folder)
{
    std::string vfspath;
    std::string ourvfspath;

    recursive_directory_iterator recur_it(folder);

    while (recur_it != recursive_directory_iterator())
    {
        if (is_regular_file(recur_it->path()))
        {
            std::string full_path = recur_it->path().string();

            vfspath = full_path.data() + _VFSRoot.length() + 1;

            ourvfspath = full_path.data() + _GameRoot.length() + 1;

            replace(vfspath.begin(), vfspath.end(), '\\', '/');

            replace(ourvfspath.begin(), ourvfspath.end(), '\\', '/');

            _Files.emplace(vfspath, ourvfspath);

        }

        ++recur_it;
    }

}

void CVirtualPaths::EnumerateOurFiles()
{
    _Files.clear();

    _GameRoot = g_plugin.GetGameDirectory().string();
    _VFSRoot = (g_plugin.GetPluginDirectory() / "eu4chs").string();

    EnumerateFolder(_VFSRoot);
}

void *CVirtualPaths::VFSOpenFileHook(const char *VirtualPath)
{
    auto it = g_VirtualFiles._Files.find(VirtualPath);

    if (it != g_VirtualFiles._Files.end())
    {
        VirtualPath = it->second.c_str();
    }

    return injector::cstd<void *(const char *)>::call(g_game.pfPHYSFS_openRead, VirtualPath);
};

void CVirtualPaths::InitAndPatch()
{
    g_VirtualFiles.EnumerateOurFiles();
    injector::MakeCALL(g_pattern.find_first("81 EC AC 01 00 00 53 56 8B F1").integer(0x8D - 0x18), VFSOpenFileHook);
}
