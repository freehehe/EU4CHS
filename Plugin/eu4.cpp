#include "stdinc.h"
#include "eu4.h"
#include "byte_pattern.h"

EU4Meta::EU4Meta()
{
    g_pattern.set_module();

    this->pfConvertUTF8ToLatin1 = g_pattern.set_pattern("81 EC B0 00 00 00 53 56 8B F1 8B DA").force_search().get(0).address(-0x18);

    this->pfCBitmapFont_GetWidthOfString = g_pattern.set_pattern("55 8B EC 83 E4 F8 81 EC 8C 00 00 00").force_search().get(0).address();

    this->pfCBitmapFont_RenderToScreen = g_pattern.set_pattern("81 EC 1C 06 00 00 56").force_search().get(0).address(-0x18);
    this->pOriginalText = *g_pattern.get(0).pointer<char *>(0xD1);
    this->pProcessedText = *g_pattern.get(0).pointer<char *>(0xFB);
    this->pWord = *g_pattern.get(0).pointer<char *>(0x10C);

    this->pfVFSOpenFile= g_pattern.set_pattern("81 EC AC 01 00 00 53 56 8B F1").force_search().get(0).address(-0x18);
    this->pfPHYSFS_openRead = GetProcAddress(pattern_default_module, "PHYSFS_openRead");

    this->ppMasterContext = g_pattern.set_pattern("6A 38 6A 00 89 3D").force_search().get(0).pointer<SMasterContextDX9 *>(6);
}

EU4Meta game_meta;
