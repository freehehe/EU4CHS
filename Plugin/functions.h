#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
    void ConvertLatin1ToUTF8(char *source);
    bool IsLatin1Char(uint32_t cp);
    bool IsTextIconChar(uint32_t cp);
    uint32_t GetNextDrawableUnicode(const char *pText, bool bUseSpecialChars);

    void InitAndPatch();
};
