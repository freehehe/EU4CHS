#include "stdinc.h"
#include "cjk_fonts.h"
#include "plugin.h"
#include "byte_pattern.h"

using namespace std;
using namespace std::experimental;

void CJKFontManager::LoadFonts()
{
    filesystem::directory_iterator dirit{ CSingleton<CPlugin>::Instance().GetPluginDirectory() / "eu4chs/fonts" };

    while (dirit != filesystem::directory_iterator{})
    {
        filesystem::path _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".fnt")
        {
            _fonts.emplace(hash<string_view>()(_path.stem().string().c_str()), _path);
        }

        ++dirit;
    }
}

void *CJKFontManager::InitGfxAndLoadTextures(void *pInfo, void *pBool)
{
    void *pMasterContext = injector::cstd<void *(void *, void *)>::call(game_meta.pfGfxInitDX9, pInfo, pBool);

    __asm
    {
        mov eax, pMasterContext;
        mov game_meta.pMasterContext, eax;
        mov eax, [eax + 4];
        mov game_meta.pDX9Device, eax;
    }

    for (auto &font : CSingleton<CJKFontManager>::Instance()._fonts)
    {
        font.second.LoadTexturesDX9();
    }

    return pMasterContext;
}

void CJKFontManager::UnloadTexturesAndShutdownGfx(void *pMasterContext)
{
    for (auto &font : CSingleton<CJKFontManager>::Instance()._fonts)
    {
        font.second.UnloadTexturesDX9();
    }

    injector::cstd<void(void *)>::call(game_meta.pfGfxShutdownDX9, pMasterContext);
}

void CJKFontManager::DrawAll(void *pMasterContext)
{
    injector::cstd<void(void*)>::call(game_meta.pfGfxDrawDX9, pMasterContext);

    for (auto &font : CSingleton<CJKFontManager>::Instance()._fonts)
    {
        font.second.DrawAllDX9();
    }
}

CJKFont * CJKFontManager::GetFont(const CString * fontname)
{
    const char *cname = fontname->c_str();

    auto it = _fonts.find(hash<string_view>()(strrchr(cname, '/') + 1));

    if (it == _fonts.end())
    {
        return &it->second;
    }
    else
    {
        return &_fonts.find(hash<string_view>()("Default"))->second;
    }
}

void CJKFontManager::InitAndPatch()
{
    LoadFonts();

    injector::WriteMemory(g_pattern.set_pattern("83 F9 02 0F 85 35 03 00 00").force_search().get(0).address(0x344), InitGfxAndLoadTextures, true);
    injector::WriteMemory(g_pattern.get(0).address(0x34E), UnloadTexturesAndShutdownGfx, true);
    injector::WriteMemory(*g_pattern.get(0).pointer<std::uintptr_t>(0x340), InitGfxAndLoadTextures, true);
    injector::WriteMemory(*g_pattern.get(0).pointer<std::uintptr_t>(0x34A), UnloadTexturesAndShutdownGfx, true);
}
