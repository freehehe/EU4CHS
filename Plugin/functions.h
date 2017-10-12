#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
    bool IsLatin1Char(uint32_t cp);
    bool IsTextIconChar(uint32_t cp);

    void GetTwoUnicode(const char * pText, uint32_t index, uint32_t text_length, uint32_t &first, ptrdiff_t &first_length, uint32_t &second, bool bUseSpecialChars);

    void InitAndPatch();
};
