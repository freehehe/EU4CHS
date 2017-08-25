#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace BitmapFont
{
    int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);

    float __fastcall GetKerning(CBitmapFontCharacterSet *pSet, int edx, uint32 first, uint32 second);

    void InitAndPatch();
}
