#pragma once
#include "stdinc.h"

class VFSManager
{
    std::unordered_map<std::string, std::string> files;

public:
    void EnumerateOurFiles();
    void InitAndPatch();
};
