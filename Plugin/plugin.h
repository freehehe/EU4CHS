#pragma once
#include "stdinc.h"

class CPlugin
{
    std::experimental::filesystem::path _game_dir;
    std::experimental::filesystem::path _plugin_dir;

public:
    void InitAndPatch(HMODULE hself);

    const std::experimental::filesystem::path &GetGameDirectory() const;
    const std::experimental::filesystem::path &GetPluginDirectory() const;
};

extern CPlugin g_plugin;
