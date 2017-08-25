#pragma once
#include "stdinc.h"
#include "NonLatinFont.h"

class NonLatinFontManager
{
    std::unordered_map<std::size_t, NonLatinFont> _fonts;

    void LoadFonts();

public:
    static void AfterDX9DeviceCreate();
    static void BeforeDX9DeviceRelease();
    NonLatinFont *GetFont(const CString *fontname);

    void InitAndPatch();
};

