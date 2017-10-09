#pragma once
#include "stdinc.h"

struct CBitmapFont;
struct CBitmapCharacterSet;
class CJKFont;

struct HookContext
{
    CBitmapFont *pFont;
    CBitmapCharacterSet *pSet;

    uint32_t textLength;

    wchar_t unicode;
    ptrdiff_t unicodeLength;

    wchar_t nextUnicode;

    bool useSpecialChars;

    void *ret_addr;
};

extern HookContext g_context;
