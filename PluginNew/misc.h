#pragma once
#include "bitmapfont.h"

namespace Misc
{
    struct HookContext
    {
        BitmapFont::CBitmapFont *pFont;
        BitmapFont::CBitmapCharacterSet *pSet;

        std::uint32_t unicode;
        std::ptrdiff_t unicodeLength;

        std::uint32_t nextUnicode;

        bool useSpecialChars;

        void *ret_addr;
    };

    extern HookContext context;

    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
    bool IsLatin1Char(std::uint32_t cp);
    void GetTwoUnicode(const char * pText, std::uint32_t index);

    void InitAndPatch();
}
