#pragma once
#include "stdinc.h"

class CTexture
{
public:
	static void *GetCHSTexture();
	static void LoadTextureCallback();
	static void UnloadTextureCallback();

	static void Patch();
};
