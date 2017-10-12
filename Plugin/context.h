#pragma once
#include "stdinc.h"

struct CBitmapFont;
struct CBitmapCharacterSet;

struct HookContext
{
    CBitmapFont *pFont;
    CBitmapCharacterSet *pSet;

    uint32_t textLength;

    uint32_t unicode;
    ptrdiff_t unicodeLength;

    uint32_t nextUnicode;

    bool useSpecialChars;

    void *ret_addr;
};

extern HookContext g_context;
