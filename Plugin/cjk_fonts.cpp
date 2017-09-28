#include "cjk_fonts.h"
#include "cjk_normal.h"
#include "cjk_map.h"
#include "plugin.h"

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

            if (filestem == "Mapfont")
            {
                _Fonts.emplace(filestem.string(), make_unique<CJKMapFont>(_path));
            }
            else
            {
                _Fonts.emplace(filestem.string(), make_unique<CJKNormalFont>(_path));
            }

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

CJKFontBase * CJKFontManager::GetFont(const CString * fontname)
{
    auto it = _Fonts.find(filesystem::path{ fontname->c_str() }.stem().string());

    if (it != _Fonts.end())
    {
        return it->second.get();
    }
    else 
    {
        return _Fonts.find("vic_18")->second.get();
    }
}

int __fastcall CJKFontManager::AddTextureHook(void *pTextureHandler, int edx, const CString *TextureFileName, void *Settings, bool bLoadTexture, bool bSaveAlpha)
{
    CJKFontBase *pFont = g_Fonts.GetFont(TextureFileName);

    pFont->LoadTexturesDX9();

    return injector::thiscall<int(void *, const CString *, void *, bool, bool)>::call(g_game.pfCTextureHandler_AddTexture, pTextureHandler, TextureFileName, Settings, bLoadTexture, bSaveAlpha);
}

struct RemoveTextureHook
{
    void operator()(injector::reg_pack *regs) const
    {
        CBitmapFont *pFont = (CBitmapFont *)regs->edi;

        g_Fonts.GetFont(pFont->GetFontPath())->UnloadTexturesDX9();

        injector::thiscall<int(uint32_t, int, void *, int)>::call(g_game.pfCTextureHandler_RemoveTextureInternal, regs->esi, *(uint32_t *)(regs->ebp - 0x10), nullptr, 0);
    }
};

void CJKFontManager::InitAndPatch()
{
    LoadFontsData();

    //_GfxSetRenderType
    g_pattern.find_pattern("83 F9 02 0F 85 35 03 00 00");

    //injector::WriteMemory(g_pattern.get(0).address(0x344), OnDX9Init, true);
    injector::WriteMemory(*g_pattern.get(0).raw<std::uintptr_t>(0x340), OnDX9Init, true);

    g_pattern.find_pattern("6A 00 6A 00 FF 75 F0 8B CE E8"); //10959CF
    injector::MakeInline<RemoveTextureHook>(g_pattern.get(0).raw(), g_pattern.get(0).raw(14));

    injector::MakeCALL(g_pattern.find_first("E8 ? ? ? ? 8B 4F 30 89 87 E0 04 00 00").raw(), AddTextureHook); //10963EE
}
