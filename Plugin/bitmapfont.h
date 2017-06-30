#pragma once
#include "eu4.h"

struct CBitMapFontCharacterValue
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
VALIDATE_SIZE(CBitMapFontCharacterValue,0x10)

struct CBitMapFontCharacterSet :IncompleteClass
{
	CBitMapFontCharacterValue **field0()
	{
		return field<CBitMapFontCharacterValue *, 0>();
	}

	float *field428()
	{
		return field<float, 0x428>();
	}
};

struct CBitmapFont :IncompleteClass
{
	CBitMapFontCharacterSet *fieldB4()
	{
		return field<CBitMapFontCharacterSet, 0xB4>();
	}

	CBitMapFontCharacterValue *GetValueByCodePoint(uint32 cp);

	static int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
	static bool IsNativeCharacter(uint32 cp);
	static void Patch();
};

