#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace BitmapFont
{
    int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
    void InitAndPatch();
}
