#include "eu4.h"
#include "../include/pattern/byte_pattern.h"
#include "../include/injector/injector.hpp"

EU4Game::EU4Game()
{
	g_pattern.set_module();

	this->pfCTextureHandler_LoadTexture;
	this->pfCTextureHandler_GetTexture;
	this->pfCTextureHandler_UnloadTexture;
	this->pfCBitMapFont_GetKerning = g_pattern.set_pattern("55 8B EC 56 8B F1 0F B6 4D 08").force_search().get(0).address();
	this->pfConvertUTF8ToLatin1 = g_pattern.set_pattern("55 8B EC 6A FF 64 A1 00 00 00 00 68 ? ? ? ? 50 64 89 25 00 00 00 00 81 EC B0 00 00 00 53 56 8B F1 8B DA 57 80 3E 00").force_search().get(0).address();
	this->pfCBitMapFont_GetWidthOfString = g_pattern.set_pattern("55 8B EC 83 E4 F8 81 EC 8C 00 00 00").force_search().get(0).address();
	this->pfCBitMapFont_GetHeightOfString;
	this->pfCBitMapFont_RenderToScreen = g_pattern.set_pattern("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 EC 1C 06 00 00").force_search().get(0).address();
	this->pfCBitMapFont_RenderToTexture = g_pattern.set_pattern("55 8B EC 6A FF 68 ? ? ? ? 64 A1 00 00 00 00 50 64 89 25 00 00 00 00 81 EC 54 05 00 00").force_search().get(0).address();

	poriginal_text = *(char **)(this->pfCBitMapFont_RenderToScreen + 0xD1);
	pword= *(char **)(this->pfCBitMapFont_RenderToScreen + 0x10C);
}

EU4Game game;
