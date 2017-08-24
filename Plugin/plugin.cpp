#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "vfs.h"
#include "byte_pattern.h"

void CPlugin::InitAndPatch(HMODULE hself)
{
    std::experimental::filesystem::path plugin_dir;

    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    plugin_dir = std::experimental::filesystem::path(module_path).parent_path();

    _font_path = plugin_dir / "eu4chs/normal.fnt";
    _mapfont_path = plugin_dir / "eu4chs/map.fnt";
    _plugin_vfs_dir = plugin_dir / "eu4chs/vfsroot/";

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    _game_dir = std::experimental::filesystem::path(module_path).parent_path();

    BitmapFont::InitAndPatch();
    Functions::InitAndPatch();
    CSingleton<VFSManager>::Instance().InitAndPatch();

    //纹理大小检测
    injector::WriteMemory<uint32_t>(g_pattern.set_pattern("81 FE 00 00 00 01").force_search().get(0).address(2), INT32_MAX, true);

    //跳过校验
    injector::MakeNOP(g_pattern.set_pattern("0F 94 45 F3 56").force_search().get(0).address(), 4, true);
}

const std::experimental::filesystem::path &CPlugin::GetFontPath() const
{
    return _font_path;
}

const std::experimental::filesystem::path &CPlugin::GetMapFontPath() const
{
    return _mapfont_path;
}

const std::experimental::filesystem::path &CPlugin::GetVFSDirectory() const
{
    return _plugin_vfs_dir;
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return _game_dir;
}
