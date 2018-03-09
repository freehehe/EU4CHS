#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "province.h"

CPlugin g_plugin;

void CPlugin::InitAndPatch(HMODULE hself)
{
    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    PluginDir = std::experimental::filesystem::path(module_path).parent_path();

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    GameDir = std::experimental::filesystem::path(module_path).parent_path();

    Functions::InitAndPatch();
    CBitmapFont::InitAndPatch();
    Province::InitAndPatch();
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return GameDir;
}

const std::experimental::filesystem::path & CPlugin::GetPluginDirectory() const
{
    return PluginDir;
}
