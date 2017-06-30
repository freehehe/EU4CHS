#pragma once
#include "stdinc.h"

class CCharTable
{
public:
	static void ReadTable(const char *filename);

	static unsigned char GetRow(uint32 cp);
	static unsigned char GetColumn(uint32 cp);
};
