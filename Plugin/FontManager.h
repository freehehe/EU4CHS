#pragma once
#include "stdinc.h"
#include "NonLatinFont.h"

class NonLatinFontManager
{
    std::unordered_map<std::size_t, NonLatinFont> _fonts;

    void LoadFonts();

public:
    static void *InitGfxAndLoadTextures(void *, void *);
    static void UnloadTexturesAndShutdownGfx(void *);
    NonLatinFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

