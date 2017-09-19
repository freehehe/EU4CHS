#pragma once
#include "stdinc.h"
#include "cjk_font.h"
#include "cjk_mapfont.h"

class CJKFontManager
{
    std::unordered_map<std::string, CJKFont> _NormalFonts;
    std::unique_ptr<CJKMapFont> _MapFont;

    static constexpr auto aa = sizeof(std::string);

    void LoadFonts();

public:
    static void *InitGfxAndLoadTextures(void *, void *);
    static void UnloadTexturesAndShutdownGfx(void *);
    static void DrawNormalFontsDX9(void *, int, int);
    static void DrawMapFontDX9();

    CJKFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

extern CJKFontManager g_Fonts;
