﻿#pragma once
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
	CBitmapFontCharacterValue **field0()
	{
		return field<CBitmapFontCharacterValue *, 0>();
	}

	float *field428()
	{
		return field<float, 0x428>();
	}
};

struct CBitmapFont :IncompleteClass
{
	//field_9C : path name like gfx/fonts/vic18

	//field_30->field_334 pointer to CTextureHandler

	CBitmapFontCharacterSet *fieldB4()
	{
		return field<CBitmapFontCharacterSet, 0xB4>();
	}

	CBitmapFontCharacterValue *GetValueByCodePoint(uint32 cp);
	static int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);

	static void Patch();
};

