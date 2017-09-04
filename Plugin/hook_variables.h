#pragma once
#include "stdinc.h"
#include "cjk_font.h"

struct hook_variables
{
    std::vector<uint32_t> wideText;
    uint32_t *pWideText;
    CJKFont *cjkFont;
    uint32_t unicode;
    uint32_t nextUnicode;
    ptrdiff_t unicodeLength;
    ptrdiff_t stringIndex;
    void *ret_addr;
};

extern hook_variables hook_context;
