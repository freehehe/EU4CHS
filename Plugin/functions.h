#pragma once
#include "stdinc.h"
#include <vector>

struct CGlobalFunctions
{
	static void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);

	static bool IsNativeCharacter(uint32 cp);

	static void Patch();
};
