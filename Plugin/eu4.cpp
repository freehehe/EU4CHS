#include "eu4.h"

EU4Meta::EU4Meta()
{
    g_pattern.find_pattern("81 EC 1C 06 00 00 56");
    this->pOriginalText = *g_pattern.get(0).raw<char *>(0xD1 - 0x18);
    this->pText = *g_pattern.get(0).raw<char *>(0xFB - 0x18);
    this->pWord = *g_pattern.get(0).raw<char *>(0x10C - 0x18);

    this->pfCTextureHandler_AddTexture = g_pattern.find_first("83 EC 40 53 56 8B F1 32 DB").integer(-0x18);
    this->pfCTextureHandler_RemoveTextureInternal = g_pattern.find_first("57 8B F9 8B 4D 08 85 C9").integer(-5);

    this->pfGfxInitDX9 = g_pattern.find_first("81 EC E8 05 00 00 53 56 57 68 8C 02 00 00").integer(-0x18);
    this->pfGfxShutdownDX9 = g_pattern.find_first("56 8B 75 08 8B 4E 04 85 C9 74 0D").integer(-3);
    this->pfGfxDrawDX9 = g_pattern.find_first("8B 08 85 F6 7F 06").integer(-0xB);

    this->pBitmapVertices = *g_pattern.find_first("F3 0F 11 04 95 ? ? ? ? 0F 28 C5").raw<STextVertex *>(5);

    this->pfCString_Assign = g_pattern.find_first("53 8B 5D 08 56 8B F1 85 DB 74 57").integer(-3);
}

EU4Meta g_game;
HookContext g_context;
