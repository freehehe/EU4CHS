#include "bitmapfont.h"
#include "table.h"
#include "functions.h"
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"
#include <iterator>
#include <vector>
#include <algorithm>

static constexpr float TextureWidth = 4096.0f;
static constexpr float TextureHeight = 4096.0f;
static constexpr float CharacterWidth = 64.0f;
static constexpr float CharacterHeight = 64.0f;
static constexpr float TextureLines = TextureWidth / CharacterWidth;
static constexpr float TextureColumns = TextureHeight / CharacterHeight;

void CBitmapFont::Patch()
{
	injector::MakeJMP(game.pfCBitMapFont_GetWidthOfString, CBitmapFont::GetWidthOfString);
}

int __fastcall CBitmapFont::GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
{
	static const float default_width = 8.0f;
	static const int chs_width = 32;

	static std::vector<uint32> wtext;

	float vTempWidth = 0.0f;
	int nWidth = 0;
	char tag[128];

	int real_length = length < 0 ? std::strlen(text) : length;

	wtext.clear();
	utf8::utf8to32(text, text + real_length, std::back_inserter(wtext));

	auto it = wtext.begin();

	while (it != wtext.end())
	{
		uint32 cp = *it;

		CBitmapFontCharacterValue *pvalue = pFont->GetValueByCodePoint(cp);

		if (bUseSpecialChars && CGlobalFunctions::IsNativeCharacter(cp) && (cp == 0xA7 || cp == 0xA3 || cp == 0xA4 || cp == 0x40 || cp == 0x7B))
		{
			switch (cp)
			{
			case 0xA7:
				++it;
				break;

			case 0x40:
				it += 3;
				vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
				break;

			case 0xA3:
			{
				std::memset(tag, 0, 128);

				++it;

				auto len = std::count_if(it, wtext.end(),
					[](uint32 cp)
				{
					return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
				});

				utf8::utf32to8(it, it + len, std::begin(tag));

				it += len;

				vTempWidth += injector::thiscall<int(CBitmapFont*, const char *)>::vtbl<28>(pFont, tag);
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

				if (pvalue->kerning && (it + 1) != wtext.end() && CGlobalFunctions::IsNativeCharacter(cp) && CGlobalFunctions::IsNativeCharacter(*(it + 1)))
				{
					uint32 nextcp = *(it + 1);
					CBitmapFontCharacterSet *pset = pFont->fieldB4();
					float fkerning;

					_asm
					{
						push nextcp;
						push cp;
						mov ecx, pset;
						call game.pfCBitMapFont_GetKerning;
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
		chs_value.x = CCharTable::GetColumn(cp) * CharacterWidth;
		chs_value.y = CCharTable::GetRow(cp) * CharacterHeight;
		chs_value.w = CharacterWidth;
		chs_value.h = CharacterHeight;
		chs_value.xoff = 0;
		chs_value.yoff = 0;
		chs_value.xadvance = 0;
		chs_value.kerning = false;

		switch (cp)
		{
		default:
			break;
		}

		return &chs_value;
	}
}
