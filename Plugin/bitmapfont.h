#pragma once
#include "stdinc.h"
#include "eu4.h"

struct CGraphics;

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
	char field_400[0x20];
	float field_420;
	float field_428;
};

struct CFont
{
	void *field_0;
	int field_4;
	bool field_8; //use special chars
	CString field_C;
	int field_24;
	int field_28;
};
VALIDATE_SIZE(CFont,0x2C)

struct CLostDeviceInterface
{
	void *field_0;
};
VALIDATE_SIZE(CLostDeviceInterface,0x4)

struct CBitmapFont
{
	CFont field_0;
	CLostDeviceInterface field_2C;
	CGraphics *field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_44;

	struct 
	{
		void *field_0;
		int field_4;
		float field_8;
		float field_C;
		float field_10;
		float field_14;
		bool field_18;
	} field_48[3]; //size 0x1C

	CString field_9C;
	CBitMapFontCharacterSet field_B4;
	char field_24F8[800];

	static int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars);
	static bool IsNativeCharacter(uint32 cp);
	static bool IsSpecialCharacter(uint32 cp);
	static void Patch();
};
VALIDATE_SIZE(CBitmapFont,0x2850)

//GetWidthOfString
//RenderToScreen
//RenderToTexture

