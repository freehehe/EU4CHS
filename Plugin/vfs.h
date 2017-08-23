#pragma once
#include "stdinc.h"

class VFSManager
{
    std::unordered_map<std::size_t, std::string> files;

public:
    static void *VFSOpenFile_0x8D(const char *virtualpath);

    void EnumerateOurFiles();
    void InitAndPatch();
};
