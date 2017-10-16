#pragma once
#include "stdinc.h"

class CVFSHook
{
    std::unordered_map<std::string, std::string> _enumerated;

    void EnumerateFiles();

public:
    void InitAndPatch();

    static void *openRead(const char *virtual_path);
};

extern CVFSHook g_vfs;
