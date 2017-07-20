#pragma once
#include "stdinc.h"

class Plugin
{
	static void Patch();

public:
	static void Init(HMODULE hself);

	static HMODULE GetASIHandle();
	static HMODULE GetEXEHandle();

	static const char *GetTexturePath();
	static const char *GetTablePath();
};
