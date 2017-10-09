#pragma once
#include "stdinc.h"

class CVirtualPaths
{
    std::string _VFSRoot;
    std::string _GameRoot;

    std::unordered_map<std::string, std::string> _Files;

    void EnumerateFolder(const std::experimental::filesystem::path &folder);

public:
    void EnumerateOurFiles();

    static void *VFSOpenFileHook(const char *VirtualPath);

    void InitAndPatch();
};

extern CVirtualPaths g_VirtualFiles;
