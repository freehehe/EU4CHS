#pragma once
#include "stdinc.h"

struct EU4Game
{
	std::uintptr_t pfCTextureHandler_AddTexture;
	std::uintptr_t pfCTextureHandler_GetTexture;

	std::uintptr_t pfCBitmapFontCharacterSet_GetKerning;
	std::uintptr_t pfConvertUTF8ToLatin1;
	std::uintptr_t pfCBitmapFont_GetWidthOfString;
	std::uintptr_t pfCBitmapFont_GetHeightOfString;
	std::uintptr_t pfCBitmapFont_RenderToScreen;
	std::uintptr_t pfCBitmapFont_RenderToTexture;

	std::uintptr_t *pfGFXDraw;
	std::uintptr_t *pfGFXSetTexture;

	std::uintptr_t pfVFSOpenFile;
	void *pfPHYSFS_openRead;

	char *poriginal_text;
	char *pword;
	char *pprocessed_text;

	EU4Game();
};

extern EU4Game game;
