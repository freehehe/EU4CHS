#pragma once
#include "stdinc.h"
#include "cjk_font.h"

class CJKFontManager
{
    std::unordered_map<std::size_t, CJKFont> _fonts;

    void LoadFonts();

public:
    static void *InitGfxAndLoadTextures(void *, void *);
    static void UnloadTexturesAndShutdownGfx(void *);
    static void DrawAll(void *);

    CJKFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

