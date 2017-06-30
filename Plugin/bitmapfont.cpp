#include "bitmapfont.h"
#include "table.h"
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"
#include <iterator>

static std::ptrdiff_t seq_len;
static uint32 cp;


struct CBitmapFont_RenderToScreen_690_13 //1098CA0
{
	void operator()(injector::reg_pack &regs) const
	{
		cp = utf8::unchecked::peek_next(&game.poriginal_text[regs.edi]);
		regs.eax = cp;

		seq_len = utf8::internal::sequence_length(&game.poriginal_text[regs.edi]);
		std::memcpy(&game.pword[regs.esi], &game.poriginal_text[regs.edi], seq_len);
		regs.esi += seq_len;

		if (!CBitmapFont::IsNativeCharacter(cp))
		{
			regs.ecx = 0;
		}
	}
};

struct CBitmapFont_RenderToScreen_85B_9 //1098E6B
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};

struct CBitmapFont_RenderToScreen_6A9_14 //1098CB9
{
	void operator()(injector::reg_pack &regs) const
	{
		regs.edi += seq_len;
		seq_len = utf8::internal::sequence_length(&game.poriginal_text[regs.edi]);
		std::memcpy(&game.pword[regs.esi], &game.poriginal_text[regs.edi], seq_len);
		regs.esi += seq_len;
	}
};

struct CBitmapFont_RenderToScreen_6C1_2C//1098CD1
{
	void operator()(injector::reg_pack &regs) const
	{
		utf8::unchecked::iterator<char *> oldit(&game.poriginal_text[regs.edi]);
		utf8::unchecked::iterator<char *> newit(&game.poriginal_text[regs.edi]);

		std::advance(newit, 2);

		std::ptrdiff_t len = newit.base() - oldit.base();

		std::memcpy(&game.pword[regs.esi], &game.poriginal_text[regs.edi], seq_len);

		regs.edi += len;
		regs.esi += len;
	}
};

struct CBitmapFont_RenderToScreen__ //1098D9B
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};

struct CBitmapFont_RenderToScreen__
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};


void CBitmapFont::Patch()
{
	injector::MakeJMP(game.pfCBitMapFont_GetWidthOfString, CBitmapFont::GetWidthOfString);
	injector::MakeInline<CBitmapFont_RenderToScreen_690_13>(game.pfCBitMapFont_RenderToScreen + 0x690, game.pfCBitMapFont_RenderToScreen + 0x690 + 13);
}

int __fastcall CBitmapFont::GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
{
	static const float default_width = 8.0f;
	static const int chs_width = 32;

	float vTempWidth = 0.0f;
	int nWidth = 0;
	char tag[128];

	int real_length = length < 0 ? std::strlen(text) : length;

	utf8::unchecked::iterator<const char *> it(text);
	utf8::unchecked::iterator<const char *> endit(text + real_length);

	while (it != endit)
	{
		uint32 cp = *it;

		CBitMapFontCharacterValue *pvalue = pFont->GetValueByCodePoint(cp);



		++it;
	}

	return max(vTempWidth, nWidth);
}

bool CBitmapFont::IsNativeCharacter(uint32 cp)
{
	return cp <= 0xFF;
}

CBitMapFontCharacterValue *CBitmapFont::GetValueByCodePoint(uint32 cp)
{
	static CBitMapFontCharacterValue chs_value;

	if (IsNativeCharacter(cp))
	{
		return this->fieldB4()->field0()[cp];
	}
	else
	{
		//TODO: Initialize chs_value;
		//CHS character is a individual word
		return &chs_value;
	}
}
