#include "stdinc.h"
#include "eu4.h"
#include "byte_pattern.h"

EU4Game::EU4Game()
{
	g_pattern.set_module();

	this->pfCTextureHandler_AddTexture = g_pattern.set_pattern("83 EC 40 53 56 8B F1 32 DB 57 89 75 E8").get(0).address(-0x18);
	this->pfCTextureHandler_GetTexture;

	this->pfCBitmapFontCharacterSet_GetKerning = g_pattern.set_pattern("55 8B EC 56 8B F1 0F B6 4D 08").get(0).address();
	this->pfCBitmapFont_GetWidthOfString = g_pattern.set_pattern("55 8B EC 83 E4 F8 81 EC 8C 00 00 00").get(0).address();
	this->pfCBitmapFont_GetHeightOfString;
	this->pfCBitmapFont_RenderToScreen = g_pattern.set_pattern("81 EC 1C 06 00 00 56 57 8B F9").get(0).address(-0x18);
	this->pfCBitmapFont_RenderToTexture = g_pattern.set_pattern("81 EC 54 05 00 00 8B 55 18").get(0).address(-0x18);
	
	this->pfCTextBuffer_GetCharacterAt = g_pattern.set_pattern("8B 55 08 56 0F B7 C2").get(0).address(-3);

	this->pfConvertUTF8ToLatin1 = g_pattern.set_pattern("81 EC B0 00 00 00 53 56 8B F1 8B DA").get(0).address(-0x18);

	this->pfPHYSFS_openRead = GetProcAddress(default_module, "PHYSFS_openRead");

	this->pfVFSOpenFile = g_pattern.set_pattern("81 EC AC 01 00 00 53 56 8B F1").get(0).address(-0x18);

	poriginal_text = *(char **)(this->pfCBitmapFont_RenderToScreen + 0xD1);
	pprocessed_text = *(char **)(this->pfCBitmapFont_RenderToScreen + 0xFB);
	pword= *(char **)(this->pfCBitmapFont_RenderToScreen + 0x10C);
}

EU4Game game;
