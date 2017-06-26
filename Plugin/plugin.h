#pragma once
#include <windows.h>

class Plugin
{
	static void Patch();

public:
	static void Init(HMODULE hself);
	static const char *GetTexturePath();
	static const char *GetTablePath();
};
