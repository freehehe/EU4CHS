#pragma once
#include "stdinc.h"

class CBitmapFont;
class CBitmapCharacterSet;
class CJKFont;

struct HookContext
{
    CBitmapFont *pFont;
    CBitmapCharacterSet *pSet;

    CJKFont *cjkFont;

    uint32_t textLength;

    uint32_t unicode;
    ptrdiff_t unicodeLength;

    uint32_t nextUnicode;

    bool useSpecialChars;

    void *ret_addr;
};

extern HookContext g_context;
