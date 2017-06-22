#pragma once
#include "stdinc.h"
#include "eu4.h"

struct CBitMapFontCharacterSet
{
	struct CBitMapFontCharacterValue
	{
		int16 x;
		int16 y;
		int16 w;
		int16 h;
		int16 xoff;
		int16 yoff;
		int16 xadvance;
		int8 page;
	} *field_0[256];

	float field_434;
};

struct CBitMapFont
{
	bool field_8; //use special chars
	CString field_9C;
	CBitMapFontCharacterSet field_B4;
	float field_428;
	char field_24F8[800];
};

int __fastcall CBitMapFont_GetWidthOfString(CBitMapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
int __fastcall CBitMapFont_GetKerning(CBitMapFont *pFont, int edx, uint32 first, uint32 second);
bool CBitMapFont_IsNativeCharacter(uint32 cp);
bool CBitMapFont_IsSpecialCharacter(uint32 cp);

//GetWidthOfString
//RenderToScreen
//RenderToTexture

