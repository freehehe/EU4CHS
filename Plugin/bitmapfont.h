#pragma once
#include "stdinc.h"
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

struct CBitMapFontCharacterSet
{
	CBitMapFontCharacterValue **get_field0() const
	{
		return (CBitMapFontCharacterValue **)this;
	}

	float *get_field428() const
	{
		return ((float *)(std::uintptr_t)this + 0x428);
	}
};

struct CBitmapFont
{
	CBitMapFontCharacterSet *get_fieldB4() const
	{
		return (CBitMapFontCharacterSet *)((std::uintptr_t)this + 0xB4);
	}

	static int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
	static bool IsNativeCharacter(uint32 cp);
	static void Patch();
};

