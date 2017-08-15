#pragma once
#include "stdinc.h"

struct EU4Meta
{
	std::uintptr_t pfConvertUTF8ToLatin1;

	std::uintptr_t pfCBitmapFontCharacterSet_GetKerning;
	std::uintptr_t pfCBitmapFont_GetWidthOfString;
	std::uintptr_t pfCBitmapFont_GetHeightOfString;
	std::uintptr_t pfCBitmapFont_RenderToScreen;
	std::uintptr_t pfCBitmapFont_RenderToTexture;

	std::uintptr_t pfCTextBuffer_GetCharacterAt;

	std::uintptr_t pMasterContext;

	std::uintptr_t *pfGFXDraw;
	std::uintptr_t *pfGFXSetTexture;

	std::uintptr_t pfVFSOpenFile;
	void *pfPHYSFS_openRead;

	char *pOriginalText;
	char *pWord;
	char *pProcessedText;

	EU4Meta();
};

extern EU4Meta game_meta;
