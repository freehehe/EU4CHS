#include "stdinc.h"
#include "plugin.h"
#include "vfs.h"
#include "eu4.h"

void CVFSManager::EnumerateOurFiles()
{
    std::string gameroot = CSingleton<CPlugin>::Instance().GetGameDirectory().string();
    std::string ourroot = CSingleton<CPlugin>::Instance().GetVFSDirectory().string();

    std::string vfspath;
    std::string ourvfspath;
    std::string full_path;

    std::experimental::filesystem::recursive_directory_iterator recur_it(ourroot);

    while (recur_it != std::experimental::filesystem::recursive_directory_iterator())
    {
        if (std::experimental::filesystem::is_regular_file(recur_it->path()))
        {
            full_path = recur_it->path().string();

            vfspath = full_path.data() + ourroot.length() + 1;
            ourvfspath = full_path.data() + gameroot.length() + 1;

            std::replace(vfspath.begin(), vfspath.end(), '\\', '/');
            std::replace(ourvfspath.begin(), ourvfspath.end(), '\\', '/');

            files.emplace(std::hash<std::string_view>()(vfspath), ourvfspath);
        }

        ++recur_it;
    }

}

void CVFSManager::Patch()
{

}
