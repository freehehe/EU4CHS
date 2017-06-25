#pragma once
#include "stdinc.h"
#include "eu4.h"

struct CBitMapFontCharacterValues
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
VALIDATE_SIZE(CBitMapFontCharacterValues,0x10)

struct CBitMapFontCharacterSet
{
	CBitMapFontCharacterValues *field_0[256];
};

struct CBitMapFont
{
	bool field_8; //use special chars
	CString field_9C;
	CBitMapFontCharacterSet field_B4;
	char field_24F8[800];
};

int __fastcall CBitMapFont_GetWidthOfString(CBitMapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
int __fastcall CBitMapFont_GetKerning(CBitMapFont *pFont, int edx, uint32 first, uint32 second);
bool CBitMapFont_IsNativeCharacter(uint32 cp);
bool CBitMapFont_IsSpecialCharacter(uint32 cp);

//GetWidthOfString
//RenderToScreen
//RenderToTexture

