#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "vfs.h"
#include "byte_pattern.h"

void CPlugin::Init(HMODULE hself)
{
    std::experimental::filesystem::path plugin_dir;

    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    plugin_dir = std::experimental::filesystem::path(module_path).root_directory();

    _font_path = plugin_dir / "eu4chs/font.dds";
    _mapfont_path = plugin_dir / "eu4chs/mapfont.dds";
    _table_path = plugin_dir / "eu4chs/font.dat";
    _vfs_dir = plugin_dir / "eu4chs/vfsroot/";

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    _game_dir = std::experimental::filesystem::path(module_path).root_directory();

    CSingleton<CVFSManager>::Instance().EnumerateOurFiles();
    Patch();
}

const std::experimental::filesystem::path &CPlugin::GetFontPath() const
{
    return _font_path;
}

const std::experimental::filesystem::path &CPlugin::GetMapFontPath() const
{
    return _mapfont_path;
}

const std::experimental::filesystem::path &CPlugin::GetTablePath() const
{
    return _table_path;
}

const std::experimental::filesystem::path &CPlugin::GetVFSDirectory() const
{
    return _vfs_dir;
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return _game_dir;
}

void CPlugin::Patch()
{
    BitmapFont::Patch();
    Functions::Patch();

    //贴图大小检测
    injector::WriteMemory<uint32_t>(g_pattern.set_module(pattern_default_module).set_pattern("81 FE 00 00 00 01").force_search().get(0).pointer(2), 0x7FFFFFFFu, true);
}
