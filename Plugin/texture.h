#pragma once

class CTextureHandler
{
public:
	static int LoadTexture();
	static void *GetTexture(int);
	static void UnloadTexture(int);

	static void LoadTextureHook();

	static void Patch();
};
