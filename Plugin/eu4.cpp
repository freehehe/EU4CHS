#include "stdinc.h"
#include "eu4.h"
#include "byte_pattern.h"

EU4Meta::EU4Meta()
{
    this->pfCBitmapFont_RenderToScreen = g_pattern.set_pattern("81 EC 1C 06 00 00 56").force_search().get(0).address(-0x18);
    this->pOriginalText = *g_pattern.get(0).pointer<char *>(0xD1);
    this->pText = *g_pattern.get(0).pointer<char *>(0xFB);
    this->pWord = *g_pattern.get(0).pointer<char *>(0x10C);

    this->pfCbitmapFontCharacterSet_GetKerning = g_pattern.find_pattern("56 8B F1 0F B6 4D 08").get(0).address(-3);

    this->pfPHYSFS_openRead = GetProcAddress(pattern_default_module, "PHYSFS_openRead");

    this->pfGfxInitDX9 = g_pattern.set_pattern("81 EC E8 05 00 00 53 56 57 68 8C 02 00 00").force_search().get(0).address(-0x18);
    this->pfGfxShutdownDX9 = g_pattern.set_pattern("56 8B 75 08 8B 4E 04 85 C9 74 0D").force_search().get(0).address(-3);
}

EU4Meta game_meta;
