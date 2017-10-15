#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "vfs.h"
#include "province.h"

CPlugin g_plugin;

void CPlugin::InitAndPatch(HMODULE hself)
{
    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    _plugin_dir = std::experimental::filesystem::path(module_path).parent_path();
    _vfs_dir = _plugin_dir / "eu4chs";

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    _game_dir = std::experimental::filesystem::path(module_path).parent_path();

    VFSHook::InitAndPatch();
    Functions::InitAndPatch();
    CBitmapFont::InitAndPatch();
    Province::InitAndPatch();
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return _game_dir;
}

const std::experimental::filesystem::path & CPlugin::GetPluginDirectory() const
{
    return _plugin_dir;
}

const std::experimental::filesystem::path & CPlugin::GetVFSDirectory() const
{
    return _vfs_dir;
}
