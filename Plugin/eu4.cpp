#include "eu4.h"

EU4Meta::EU4Meta()
{
	g_pattern.find_pattern("68 00 0C 00 00 50 68");
	if (g_pattern.has_size(1))
		this->pOriginalText = *g_pattern.get_first().pointer<char *>(7);

	g_pattern.find_pattern("68 3C 0C 00 00 6A 00 68");
	if (g_pattern.has_size(1))
		this->pText = *g_pattern.get_first().pointer<char *>(8);

	g_pattern.find_pattern("68 00 01 00 00 6A 00 68");
	if (g_pattern.has_size(1))
		this->pWord = *g_pattern.get_first().pointer<char *>(8);

	g_pattern.find_pattern("53 8B 5D 08 56 8B F1 85 DB 74 57");
	if (g_pattern.has_size(1))
		this->pfCString_Assign = g_pattern.get_first().integer(-3);
}

EU4Meta g_game;
