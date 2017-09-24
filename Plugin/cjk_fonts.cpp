#include "stdinc.h"
#include "cjk_fonts.h"
#include "cjk_mapfont.h"
#include "plugin.h"
#include "byte_pattern.h"

CJKFontManager g_Fonts;

using namespace std;
using namespace std::experimental;

void CJKFontManager::LoadFonts()
{
    filesystem::directory_iterator dirit{ g_plugin.GetPluginDirectory() / "eu4chs" };

    while (dirit != filesystem::directory_iterator{})
    {
        filesystem::path _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".fnt")
        {
            auto filestem = _path.stem();

            if (filestem == "Mapfont" && !_MapFont)
            {
                _MapFont = make_unique<CJKMapFont>(_path);
            }
            else
            {
                _NormalFonts.emplace(filestem.string(), _path);
            }

        }

        ++dirit;
    }
}

void *CJKFontManager::InitGfxAndLoadTextures(void *pInfo, void *pBool)
{
    void *pMasterContext = injector::cstd<void *(void *, void *)>::call(g_game.pfGfxInitDX9, pInfo, pBool);

    __asm
    {
        mov eax, pMasterContext;
        mov eax, [eax + 4];
        mov g_game.pDX9Device, eax;
    }

    for (auto &font : g_Fonts._NormalFonts)
    {
        font.second.LoadTexturesDX9();
    }

    g_Fonts._MapFont->LoadTexturesDX9();

    return pMasterContext;
}

void CJKFontManager::UnloadTexturesAndShutdownGfx(void *pMasterContext)
{
    for (auto &font : g_Fonts._NormalFonts)
    {
        font.second.UnloadTexturesDX9();
    }

    g_Fonts._MapFont->UnloadTexturesDX9();

    injector::cstd<void(void *)>::call(g_game.pfGfxShutdownDX9, pMasterContext);
}

void CJKFontManager::DrawNormalFontsDX9(void *a1, int a2, int a3)
{
    injector::cstd<void(void *, int, int)>::call(g_game.pfGfxDrawDX9, a1, a2, a3);

    LPDIRECT3DBASETEXTURE9 original;

    g_game.pDX9Device->GetTexture(0, &original);

    for (auto &font: g_Fonts._NormalFonts)
    {
        font.second.DrawAllDX9();
    }

    g_game.pDX9Device->SetTexture(0, original);
}

void CJKFontManager::DrawMapFontDX9()
{
    LPDIRECT3DBASETEXTURE9 original;

    g_game.pDX9Device->GetTexture(0, &original);

    g_Fonts._MapFont->DrawAllDX9();

    g_game.pDX9Device->SetTexture(0, original);
}

CJKFont * CJKFontManager::GetFont(const CString * fontname)
{
    //暂时先按文件名查找
    const char *cname = (strrchr(fontname->c_str(), '/') + 1);

    auto it = _NormalFonts.find(cname);

    if (it != _NormalFonts.end())
    {
        return &it->second;
    }
    else 
    {
        return &_NormalFonts.find("vic_18")->second;
    }
}

void CJKFontManager::InitAndPatch()
{
    LoadFonts();

    //_GfxSetRenderType
    g_pattern.find_pattern("83 F9 02 0F 85 35 03 00 00");

    //injector::WriteMemory(g_pattern.get(0).address(0x344), InitGfxAndLoadTextures, true);
    injector::WriteMemory(*g_pattern.get(0).raw<std::uintptr_t>(0x340), InitGfxAndLoadTextures, true);

    //injector::WriteMemory(g_pattern.get(0).address(0x34E), UnloadTexturesAndShutdownGfx, true);
    injector::WriteMemory(*g_pattern.get(0).raw<std::uintptr_t>(0x34A), UnloadTexturesAndShutdownGfx, true);

    //injector::WriteMemory(g_pattern.get(0).address(0x3E4), InitGfxAndLoadTextures, true);
    injector::WriteMemory(*g_pattern.get(0).raw<std::uintptr_t>(0x3E0), DrawNormalFontsDX9, true);
}
