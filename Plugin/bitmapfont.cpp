#include "bitmapfont.h"
#include <cstring>
#include <vector>
#include <algorithm>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"
#include <xmmintrin.h>
#include "../include/pattern/byte_pattern.h"

int __fastcall CBitMapFont_GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
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

	CBitMapFontCharacterSet *pset = (memory_pointer_raw(pFont) + 0xB4).get();
	CBitMapFontCharacterValues **ppvalues = memory_pointer_raw(pset).get();

	while (it < endit)
	{
		uint32 cp = utf8::next(it, endit);

		if (!CBitMapFont_IsNativeCharacter(cp))
		{
			//中文字符
			vTempWidth += chs_width * (*((memory_pointer_raw(pset) + 0x428).get<float>()));
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
					vTempWidth += ppvalues[cp]->xadvance * (*((memory_pointer_raw(pset) + 0x428).get<float>()));

					if (ppvalues[cp]->kerning && it < endit)
					{
						uint32 nextcp = utf8::peek_next(it, endit);

						if (CBitMapFont_IsNativeCharacter(nextcp))
						{
							static void *pfgetkerning = g_pattern.set_pattern("").force_search().get(0).pointer();

							float fkerning;

							__asm
							{
								push nextcp;
								push cp;
								mov ecx, pset;
								call EU4Game.pfCBitMapFont_GetKerning;
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

bool CBitMapFont_IsNativeCharacter(uint32 cp)
{
	return cp <= 0xFF;
}
