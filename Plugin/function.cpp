#include "function.h"
#include <cstring>
#include "../include/utf8cpp/utf8.h"

void UTF8ToLatin1(const char *source, char *dest)
{
	std::strcpy(dest, source);
}

void UTF8ToLatin1(char *dest, const char *source)
{
	std::strcpy(dest, source);
}
