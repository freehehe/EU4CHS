#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace BitmapFont
{
    //获取字符串的宽度，用于计算背景宽度
    int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);

    void InitAndPatch();
}
#pragma message("Fuck you")
