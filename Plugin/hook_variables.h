#pragma once
#include "stdinc.h"
#include "cjk_font.h"

struct hook_variables
{
    std::vector<uint32_t> wideText;
    CJKFont *cjkFont;
    uint32_t unicode;
    uint32_t nextUnicode;
    ptrdiff_t unicodeLength;
    void *ret_addr;
};

extern hook_variables g_context;
