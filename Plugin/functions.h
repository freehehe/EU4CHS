#pragma once
#include "stdinc.h"
#include <vector>

struct CGlobalFunctions
{
	static void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
	static void ConvertUTF8ToUCS4(const char *source, std::vector<uint32> &dest);

	static bool IsNativeCharacter(uint32 cp);

	static void Patch();
};

