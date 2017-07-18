#include "eu4.h"
#include "../include/pattern/byte_pattern.h"

EU4Game::EU4Game()
{
	g_pattern.set_module();

	this->pfCTextureHandler_AddTexture = g_pattern.set_pattern("83 EC 40 53 56 8B F1 32 DB 57 89 75 E8").get(0).address(-0x18);
	this->pfCTextureHandler_GetTexture;

	this->pfCBitmapFontCharacterSet_GetKerning = g_pattern.set_pattern("55 8B EC 56 8B F1 0F B6 4D 08").get(0).address();
	this->pfCBitmapFont_GetWidthOfString = g_pattern.set_pattern("55 8B EC 83 E4 F8 81 EC 8C 00 00 00").get(0).address();
	this->pfCBitmapFont_GetHeightOfString;
	this->pfCBitmapFont_RenderToScreen = g_pattern.set_pattern("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 EC 1C 06 00 00").get(0).address();
	this->pfCBitmapFont_RenderToTexture = g_pattern.set_pattern("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 EC 54 05 00 00").get(0).address();
	
	this->pfPHYSFS_OpenRead = GetProcAddress(default_module, "PHYSFS_openRead");

	poriginal_text = *(char **)(this->pfCBitmapFont_RenderToScreen + 0xD1);
	pprocessed_text = *(char **)(this->pfCBitmapFont_RenderToScreen + 0xFB);
	pword= *(char **)(this->pfCBitmapFont_RenderToScreen + 0x10C);
}

EU4Game game;
