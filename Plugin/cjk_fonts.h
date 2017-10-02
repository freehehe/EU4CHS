#pragma once
#include "stdinc.h"
#include "cjk_font.h"

class CString;

class CJKFontManager
{
    std::unordered_map<std::string, CJKFont> _Fonts;

    void LoadFontsData();

public:
    static void *OnDX9Init(void *, void *);

    static void __fastcall AddTextureHook(CBitmapFont *pFont);

    CJKFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

extern CJKFontManager g_Fonts;
