#pragma once
#include "stdinc.h"


class CCharTable
{
public:
	static void read_table(const char *filename);

	static	unsigned char find_row(uint32 cp);
	static unsigned char find_column(uint32 cp);
};
