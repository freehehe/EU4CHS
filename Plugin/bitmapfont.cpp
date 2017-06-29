#include "bitmapfont.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"
#include <xmmintrin.h>
#include "../include/pattern/byte_pattern.h"

static std::ptrdiff_t seq_len;
static uint32 cp;
static CBitMapFontCharacterValue chs_value;

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
		CBitMapFontCharacterSet *pset = *(CBitMapFontCharacterSet **)(regs.ebp - 0x34);
		CBitMapFontCharacterValue **pvalues = (CBitMapFontCharacterValue **)pset;

		if (CBitmapFont::IsNativeCharacter(regs.eax))
		{
			regs.ecx = (uint32)pset->get_field0()[regs.eax];
		}
		else
		{
			//TODO: Initialize chs_value;
			chs_value.xadvance = 64;
			chs_value.kerning = false;
			regs.ecx = (uint32)&chs_value;
		}
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
	using namespace injector;

	static const float default_width = 8.0f;
	static const int chs_width = 32;

	float vTempWidth = 0.0f;
	int nWidth = 0;
	char tag[128];

	int real_length = length < 0 ? std::strlen(text) : length;

	const char *it = text;
	const char *endit = text + real_length;

	CBitMapFontCharacterSet *pset = pFont->get_fieldB4();
	CBitMapFontCharacterValue **ppvalues = pset->get_field0();

	while (it < endit)
	{
		uint32 cp = utf8::next(it, endit);

		if (!IsNativeCharacter(cp))
		{
			//中文字符
			vTempWidth += chs_width * *pset->get_field428();
		}
		else
		{
			//原版字符
			if (bUseSpecialChars && (cp == 0x40 || cp == 0x7B || cp == 0xA3 || cp == 0xA4 || cp == 0xA7))
			{
				int special_width;

				if (cp == 0xA7)
				{
					continue;
				}

				if (cp == 0x40)
				{
					it += 3;
					special_width = thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
				}
				else if (cp == 0xA3)
				{
					std::size_t index = 0;

					while (it <= endit)
					{
						if (isalpha(*it) || isdigit(*it) || *it == '_' || *it == '|' || index < sizeof(tag) - 1)
						{
							tag[index] = *it;
							++it;
						}
						else
						{
							break;
						}
					}

					tag[index] = 0;

					special_width = thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, tag);
				}
				else if (cp == 0xA4)
				{
					vTempWidth += default_width;
					continue;
				}
				else if (cp == 0x7B)
				{
					it += 2;
					vTempWidth += default_width;
					continue;
				}

				vTempWidth += special_width;
			}
			else
			{
				if (ppvalues[cp])
				{
					vTempWidth += ppvalues[cp]->xadvance * *pset->get_field428();

					if (ppvalues[cp]->kerning && it < endit)
					{
						uint32 nextcp = utf8::peek_next(it, endit);

						if (IsNativeCharacter(nextcp))
						{
							static void *pfgetkerning = g_pattern.set_pattern("").force_search().get(0).pointer();

							float fkerning;

							__asm
							{
								push nextcp;
								push cp;
								mov ecx, pset;
								call game.pfCBitMapFont_GetKerning;
								movss fkerning, xmm0;
							}

							vTempWidth += fkerning;
						}
					}

					if (ppvalues[cp]->h == 0)
					{
						nWidth = max(nWidth, vTempWidth);
					}
				}
				else if (cp == 0xA)
				{
					if (vTempWidth > nWidth)
					{
						nWidth = max(vTempWidth, nWidth);
					}

					vTempWidth = 0.0f;
				}
			}
		}
	}

	return max(vTempWidth, nWidth);
}

bool CBitmapFont::IsNativeCharacter(uint32 cp)
{
	return cp <= 0xFF;
}
