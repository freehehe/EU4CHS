#include "functions.h"
#include <cstring>
#include "../include/utf8cpp/utf8.h"

void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest)
{
	std::strcpy(dest, source);
}
