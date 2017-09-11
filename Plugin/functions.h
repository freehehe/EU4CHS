#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
    bool IsLatin1Char(uint32_t cp);
    bool IsTextIconChar(uint32_t cp);
    uint32_t GetNextUnicode(const char *pText, bool bUseSpecialChars);
    bool IsSpecialChar(uint32_t unicode);

    void InitAndPatch();
};
