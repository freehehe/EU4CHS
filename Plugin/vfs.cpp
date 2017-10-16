#include "vfs.h"
#include "plugin.h"
#include "eu4.h"

CVFSHook g_vfs;

using namespace std;
using namespace std::experimental::filesystem::v1;

void CVFSHook::EnumerateFiles()
{
    std::string game_root = g_plugin.GetGameDirectory().string();
    std::string vfs_root = (g_plugin.GetPluginDirectory() / "eu4chs").string();

    _enumerated.clear();

    recursive_directory_iterator dir_it{ vfs_root };

    while (dir_it != recursive_directory_iterator{})
    {
        if (is_regular_file(dir_it->path()))
        {
            string full_path = dir_it->path().string();

            replace(full_path.begin(), full_path.end(), '\\', '/');

            _enumerated.emplace(string(full_path, vfs_root.length() + 1), string(full_path, game_root.length() + 1));
        }

        ++dir_it;
    }
}

void * CVFSHook::openRead(const char *virtual_path)
{
    auto it = g_vfs._enumerated.find(virtual_path);

    if (it != g_vfs._enumerated.end())
    {
        virtual_path = it->second.c_str();
    }

    return injector::cstd<void *(const char *)>::call(g_game.pfPHYSFS_openRead, virtual_path);
}

void CVFSHook::InitAndPatch()
{
    EnumerateFiles();

    injector::MakeCALL(g_pattern.find_first("56 E8 ? ? ? ? 8B F8 C7 45 FC FF FF FF FF A1").integer(1), CVFSHook::openRead);
}
