#pragma once
#include "stdinc.h"
#include "cjk_font.h"
#include "std_string.h"

class CJKFontManager
{
    std::unordered_map<std::string, CJKFont> _Fonts;

    void LoadFontsData();

public:
    static void *OnDX9Init(void *, void *);

    static int __fastcall AddTextureHook(void *pTextureHandler, int edx, const CString *TextureFileName, void *Settings, bool bLoadTexture, bool bSaveAlpha);

    CJKFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

extern CJKFontManager g_Fonts;
