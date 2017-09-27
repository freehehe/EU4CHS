#pragma once
#include "stdinc.h"
#include "cjk_font.h"
#include "cjk_mapfont.h"

class CJKFontManager
{
    std::unordered_map<std::string, CJKFont> _NormalFonts;
    std::unique_ptr<CJKMapFont> _MapFont;

    void LoadFontsData();

public:
    static void *OnDX9Init(void *, void *);

    static int __fastcall AddTextureHook(void *pTextureHandler, int edx, const CString *TextureFileName, void *Settings, bool bLoadTexture, bool bSaveAlpha);

    static void DrawNormalFontsDX9(void *, int, int);
    static void DrawMapFontDX9();

    CJKFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

extern CJKFontManager g_Fonts;
