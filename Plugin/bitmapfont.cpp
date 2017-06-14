#include "bitmapfont.h"
#include <vector>
#include <iterator>
#include "../include/utf8cpp/utf8.h"

int __fastcall CBitMapFont_GetWidthOfString(const char *text, int edx, const int length, bool bUseSpecialChars)
{
	static std::vector<uint32> s_buffer;

	s_buffer.clear();

	utf8::utf8to32(text, text + length, std::back_inserter(s_buffer));


}
