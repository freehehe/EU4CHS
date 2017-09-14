#pragma once
#include "stdinc.h"
#include "cjk_font.h"
#include "bitmapfont.h"

struct hook_variables
{
    CBitmapFont *pFont;
    CBitmapCharacterSet *pSet;
    CJKFont *cjkFont;
    std::string narrowText;
    std::vector<uint32_t> wideText;
    uint32_t unicode;
    uint32_t nextUnicode;
    uint32_t nextDrawableUnicode;
    ptrdiff_t unicodeLength;
    void *ret_addr;
};

extern hook_variables g_context;
