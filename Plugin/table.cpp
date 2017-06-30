#include "plugin.h"
#include "table.h"
#include <utility>
#include <vector>
#include <fstream>

static std::vector<std::pair<uint8,uint8>> table;

void CCharTable::ReadTable(const char *filename)
{
	std::ifstream ifs(filename, std::ios::binary);

	table.resize(0x10000);

	if (ifs)
	{
		ifs.read(reinterpret_cast<char *>(table.data()), 0x10000 * 2);
	}
}

unsigned char CCharTable::GetRow(uint32 cp)
{
	return table[cp].first;
}

unsigned char CCharTable::GetColumn(uint32 cp)
{
	return table[cp].second;
}
