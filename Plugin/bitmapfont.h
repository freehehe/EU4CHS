#pragma once
#include "stdinc.h"
#include "eu4.h"

//Mapfont
//'M' 77
//(x,y) 字符左上角绝对坐标
//(w,h) 字符长，宽

struct CBitmapFontCharacterValue
{
    int16 x;
    int16 y;
    int16 w;
    int16 h;
    int16 xoff;
    int16 yoff;
    int16 xadvance;
    bool kerning;
};
VALIDATE_SIZE(CBitmapFontCharacterValue,0x10)

struct CBitmapFontCharacterSet :IncompleteClass
{
    CBitmapFontCharacterValue *GetValueByCodePoint(uint32 cp);

    float *field_428()
    {
        return field<float, 0x428>();
    }
};

struct CBitmapFont :IncompleteClass
{
    //field_9C : font name like gfx/fonts/vic18

    CBitmapFontCharacterSet *GetCharacterSet()
    {
        return field<CBitmapFontCharacterSet, 0xB4>();
    }

    const CString *GetFontName()
    {
        return field<const CString, 0x9C>();
    }

    CBitmapFontCharacterValue *GetValueByCodePoint(uint32 cp)
    {
        return GetCharacterSet()->GetValueByCodePoint(cp);
    }

    static int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
    static void Patch();
};

