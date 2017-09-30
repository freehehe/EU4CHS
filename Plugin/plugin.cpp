#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "cjk_fonts.h"

CPlugin g_plugin;

void CPlugin::InitAndPatch(HMODULE hself)
{
    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    _plugin_dir = std::experimental::filesystem::path(module_path).parent_path();

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    _game_dir = std::experimental::filesystem::path(module_path).parent_path();

    g_Fonts.InitAndPatch();
    Functions::InitAndPatch();
    CBitmapFont::InitAndPatch();
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return _game_dir;
}

const std::experimental::filesystem::path & CPlugin::GetPluginDirectory() const
{
    return _plugin_dir;
}
