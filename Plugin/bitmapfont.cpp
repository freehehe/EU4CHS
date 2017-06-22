#include "bitmapfont.h"
#include <cstring>
#include <vector>
#include <iterator>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/assembly.hpp"
#include <immintrin.h>

struct CBitMapFont_RenderToScreen_8C_Hook
{
	void operator()(injector::reg_pack &regs) const
	{
		*(uint32 *)(regs.ebp - 0xA4) = utf8::peek_next((const char *)regs.eax, (const char *)regs.eax + std::strlen((const char *)regs.eax));
	}
};

int __fastcall CBitMapFont_GetWidthOfString(CBitMapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
{
	static std::vector<uint32> s_buffer;

	int result;
	float fresult;

	int real_length = length < 0 ? std::strlen(text) : length;

	s_buffer.clear();

	utf8::utf8to32(text, text + real_length, std::back_inserter(s_buffer));

	auto it = s_buffer.begin();


}

bool CBitMapFont_IsNativeCharacter(uint32 cp)
{

}
