#include "cjk_fonts.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "std_string.h"

CJKFontManager g_Fonts;

using namespace std;
using namespace std::experimental;

void CJKFontManager::LoadFontsData()
{
    filesystem::directory_iterator dirit{ g_plugin.GetPluginDirectory() / "eu4chs" };

    while (dirit != filesystem::directory_iterator{})
    {
        filesystem::path _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".fnt")
        {
            auto filestem = _path.stem();

            _Fonts.emplace(filestem.string(), _path);
        }

        ++dirit;
    }
}

void *CJKFontManager::OnDX9Init(void *pInfo, void *pBool)
{
    IncompleteClass *pMasterContext = injector::cstd<IncompleteClass *(void *, void *)>::call(g_game.pfGfxInitDX9, pInfo, pBool);

    g_game.pDX9Device = pMasterContext->get_field<LPDIRECT3DDEVICE9, 4>();

    return pMasterContext;
}

void __fastcall CJKFontManager::AddTextureHook(CBitmapFont *pFont)
{
    *pFont->GetLatin1CharacterSet()->field<void *, 0>() = g_Fonts.GetFont(pFont->GetFontPath());

    pFont->GetCJKFont()->LoadTexturesDX9();

    injector::thiscall<void(CBitmapFont *)>::call(g_game.pfCBitmapFont_ParseFontFile, pFont);
}

CJKFont * CJKFontManager::GetFont(const CString * fontname)
{
    auto it = _Fonts.find(filesystem::path{ fontname->c_str() }.stem().string());

    if (it != _Fonts.end())
    {
        return &it->second;
    }
    else
    {
        return &_Fonts.find("vic_18")->second;
    }
}

struct RemoveTextureHook
{
    void operator()(injector::reg_pack &regs) const
    {
        CBitmapFont *pFont = (CBitmapFont *)regs.edi;

        pFont->GetCJKFont()->UnloadTexturesDX9();
        *pFont->GetLatin1CharacterSet()->field<void *, 0>() = nullptr; //delete crash fix

        injector::thiscall<int(void *, int, void *, int)>::call(g_game.pfCTextureHandler_RemoveTextureInternal, regs.esi.p, *(uint32_t *)(regs.ebp.i - 0x10), nullptr, 0);
    }
};

void CJKFontManager::InitAndPatch()
{
    LoadFontsData();

    //_GfxSetRenderType
    g_pattern.find_pattern("83 F9 02 0F 85 35 03 00 00");

    injector::WriteMemory(*g_pattern.get(0).pointer<std::uintptr_t>(0x340), OnDX9Init, true);

    g_pattern.find_pattern("89 87 EC 04 00 00 E8"); //109641F
    injector::MakeCALL(g_pattern.get_first().pointer(6), AddTextureHook);

    g_pattern.find_pattern("6A 00 6A 00 FF 75 F0 8B CE E8"); //10959CF
    injector::MakeInline<RemoveTextureHook>(g_pattern.get(0).pointer(), g_pattern.get(0).pointer(14));
}
