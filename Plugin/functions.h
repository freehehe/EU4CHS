#pragma once
#include "stdinc.h"

struct CGlobalFunctions
{
	static void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
	static void ConvertLatin1ToUTF8(const char *source, char *dest);
	static bool IsNativeCharacter(uint32 cp);
	static bool IsTextIconChar(uint32 cp);

	static void Patch();
};
