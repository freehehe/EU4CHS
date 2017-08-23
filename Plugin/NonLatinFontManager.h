#pragma once
#include "stdinc.h"
#include "NonLatinFont.h"

class NonLatinFontManager
{
    std::unordered_map<std::size_t, NonLatinFont> _fonts;

public:
    NonLatinFontManager();
    ~NonLatinFontManager();

    void LoadAllFonts();
};

