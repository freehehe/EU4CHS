﻿#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "vfs.h"
#include "byte_pattern.h"
#include "FontManager.h"

void CPlugin::InitAndPatch(HMODULE hself)
{
    char module_path[512];

    GetModuleFileNameA(hself, module_path, 512);
    _plugin_dir = std::experimental::filesystem::path(module_path).parent_path();

    GetModuleFileNameA(GetModuleHandle(NULL), module_path, 512);
    _game_dir = std::experimental::filesystem::path(module_path).parent_path();

    CSingleton<NonLatinFontManager>::Instance().InitAndPatch();
    CSingleton<VFSManager>::Instance().InitAndPatch();

    //纹理大小检测
    injector::WriteMemory<uint32_t>(g_pattern.set_pattern("81 FE 00 00 00 01").force_search().get(0).address(2), INT32_MAX, true);

    //跳过校验
    injector::MakeNOP(g_pattern.set_pattern("0F 94 45 F3 56").force_search().get(0).address(), 4, true);
}

const std::experimental::filesystem::path & CPlugin::GetGameDirectory() const
{
    return _game_dir;
}

const std::experimental::filesystem::path & CPlugin::GetPluginDirectory() const
{
    return _plugin_dir;
}
