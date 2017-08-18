#pragma once
#include "stdinc.h"
#include "eu4.h"

namespace Functions
{
	void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest);
	void ConvertLatin1ToUTF8(const char *source, char *dest);
	bool IsNativeCharacter(uint32 cp);
	bool IsTextIconChar(uint32 cp);

	void Patch();
};
