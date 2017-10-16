#include "eu4.h"

EU4Meta::EU4Meta()
{
    this->pOriginalText = *g_pattern.find_first("68 00 0C 00 00 50 68").pointer<char *>(7);
    this->pText = *g_pattern.find_first("68 3C 0C 00 00 6A 00 68").pointer<char *>(8);
    this->pWord = *g_pattern.find_first("68 00 01 00 00 6A 00 68").pointer<char *>(8);

    this->pfPHYSFS_openRead = GetProcAddress(GetModuleHandleW(NULL), "PHYSFS_openRead");

    this->pfCString_Assign = g_pattern.find_first("53 8B 5D 08 56 8B F1 85 DB 74 57").integer(-3);
}

EU4Meta g_game;
