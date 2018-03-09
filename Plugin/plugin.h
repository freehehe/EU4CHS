#pragma once
#include "stdinc.h"

class CPlugin
{
    std::experimental::filesystem::path GameDir;
    std::experimental::filesystem::path PluginDir;

public:
    void InitAndPatch(HMODULE hself);

    const std::experimental::filesystem::path &GetGameDirectory() const;
    const std::experimental::filesystem::path &GetPluginDirectory() const;
};

extern CPlugin g_plugin;
