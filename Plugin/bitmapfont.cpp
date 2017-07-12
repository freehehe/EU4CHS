#include "bitmapfont.h"
#include "table.h"
#include "functions.h"
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"
#include <iterator>
#include <vector>
#include <algorithm>

static const float TextureWidth = 4096.0f;
static const float TextureHeight = 4096.0f;
static const float CharacterWidth = 64.0f;
static const float CharacterHeight = 64.0f;
static const float TextureLines = TextureWidth / CharacterWidth;
static const float TextureColumns = TextureHeight / CharacterHeight;

static const float default_width = 8.0f;
static const int chs_width = 32;

int __fastcall CBitmapFont::GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
{
	static std::vector<uint32> wtext;

	float vTempWidth = 0.0f;
	int nWidth = 0;
	char tag[128];

	int real_length = length < 0 ? std::strlen(text) : length;

	wtext.clear();
	utf8::unchecked::utf8to32(text, text + real_length, std::back_inserter(wtext));

	auto it = wtext.begin();

	while (it != wtext.end())
	{
		uint32 cp = *it;

		CBitmapFontCharacterValue *pvalue = pFont->GetValueByCodePoint(cp);

		if (bUseSpecialChars && (cp == 0xA7 || cp == 0xA3 || cp == 0xA4 || cp == 0x40 || cp == 0x7B))
		{
			switch (cp)
			{
#error Fix cases here
			case 0xA7:
				++it;
				break;

			case 0x40:
				it += 3;
				vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
				break;

			case 0xA3:
			{
				std::fill(std::begin(tag), std::end(tag), 0);

				++it;

				auto len = std::count_if(it, wtext.end(), CGlobalFunctions::IsTextIconChar);

				utf8::unchecked::utf32to8(it, it + len, std::begin(tag));

				it += len;

				vTempWidth += injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, tag);
			}
				break;

			case 0xA4:
				vTempWidth += default_width;
				break;

			case 0x7B:
				it += 2;
				vTempWidth += default_width;
				break;

			default:
				break;
			}
		}
		else
		{
			CBitmapFontCharacterValue *pvalue = pFont->GetValueByCodePoint(cp);

			if (pvalue)
			{
				vTempWidth += pvalue->xadvance * *pFont->fieldB4()->field428();

				//或者可以省掉naive char判定
				if (pvalue->kerning && (it + 1) != wtext.end() && CGlobalFunctions::IsNativeCharacter(*(it + 1)))
				{
					uint32 nextcp = *(it + 1);
					CBitmapFontCharacterSet *pset = pFont->fieldB4();
					float fkerning;

					_asm
					{
						push nextcp;
						push cp;
						mov ecx, pset;
						call game.pfCBitmapFont_GetKerning;
						movss fkerning, xmm0;
					}

					vTempWidth += fkerning;
				}

				if (pvalue->h == 0 || !CGlobalFunctions::IsNativeCharacter(cp))
				{
					nWidth = max(vTempWidth, nWidth);
				}
			}
			else
			{
				if (cp == 0xA)
				{
					nWidth = max(vTempWidth, nWidth);
					vTempWidth = 0.0f;
				}
			}
		}

		++it;
	}

	return max(vTempWidth, nWidth);
}

CBitmapFontCharacterValue *CBitmapFont::GetValueByCodePoint(uint32 cp)
{
	static CBitmapFontCharacterValue chs_value;

	if (CGlobalFunctions::IsNativeCharacter(cp))
	{
		return this->fieldB4()->field0()[cp];
	}
	else
	{
		//可能要根据字号计算某些数值
		chs_value.x = CCharTable::GetColumn(cp) * CharacterWidth;
		chs_value.y = CCharTable::GetRow(cp) * CharacterHeight;
		chs_value.w = CharacterWidth;
		chs_value.h = CharacterHeight;
		chs_value.xoff = 0;
		chs_value.yoff = 0;
		chs_value.xadvance = chs_width;
		chs_value.kerning = false;

		//可能有些字符的数值需要调整
		switch (cp)
		{
		default:
			break;
		}

		return &chs_value;
	}
}

static uint32 code_point;
static std::ptrdiff_t cp_len;
static void *ret_addr;

struct CBitmapFont_RenderToScreen_0x690_13
{
	void operator()(injector::reg_pack &regs) const
	{
		char *source = game.poriginal_text + regs.edi;
		char *dest = game.pword + regs.esi;

		cp_len = utf8::internal::sequence_length(source);

		code_point = utf8::unchecked::peek_next(dest);

		regs.eax = code_point;

		utf8::unchecked::append(code_point, dest);

		regs.edi += (cp_len - 1);
		regs.esi += cp_len;

		if (!CGlobalFunctions::IsNativeCharacter(code_point))
		{
			regs.ecx = 0;
		}
	}
};

__declspec(naked) void CBitmapFont_RenderToScreen_OFF_SIZE()
{
	__asm
	{
		pop ret_addr;

	}
}

struct CBitmapFont_RenderToScreen_OFF_SIZE
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};

void CBitmapFont::Patch()
{
	injector::MakeInline<CBitmapFont_RenderToScreen_0x690_13>(game.pfCBitmapFont_RenderToScreen + 0x690, game.pfCBitmapFont_RenderToScreen + 0x690 + 13);
	injector::MakeJMP(game.pfCBitmapFont_GetWidthOfString, CBitmapFont::GetWidthOfString);
}
