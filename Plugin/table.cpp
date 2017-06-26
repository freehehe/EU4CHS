#include "plugin.h"
#include "table.h"
#include <vector>
#include <fstream>

struct CharPos
{
	uint8 row = 63;
	uint8 column = 63;
};

static std::vector<CharPos> table;

void CCharTable::read_table(const char *filename)
{
	std::ifstream ifs(filename, std::ios::binary);

	table.resize(0x10000);

	if (ifs)
	{
		ifs.read(reinterpret_cast<char *>(table.data()), 0x10000 * 2);
	}
}

unsigned char CCharTable::find_row(uint32 cp)
{
	return table[cp].row;
}

unsigned char CCharTable::find_column(uint32 cp)
{
	return table[cp].column;
}
