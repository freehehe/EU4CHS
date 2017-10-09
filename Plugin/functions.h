#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
    bool IsLatin1Char(wchar_t cp);
    bool IsTextIconChar(wchar_t cp);

    void GetTwoUnicode(const char * pText, uint32_t index, uint32_t text_length, wchar_t &first, ptrdiff_t &first_length, wchar_t &second, bool bUseSpecialChars);
    //void GetTwoUnicode(std::vector<wchar_t>::iterator pText, std::vector<wchar_t>::iterator pEnd, wchar_t &first, wchar_t &second, bool bUseSpecialChars);

    void InitAndPatch();
};
