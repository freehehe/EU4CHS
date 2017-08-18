#include "stdinc.h"
#include "table.h"

void CChsFont::ReadTable(const std::experimental::filesystem::path &filename)
{
	std::ifstream ifs(filename, std::ios::binary);

	_table.resize(0x10000);

	if (ifs)
	{
		ifs.read(reinterpret_cast<char *>(_table.data()), 0x10000 * 2);
	}
}

uint8 CChsFont::GetRow(uint32 cp) const
{
	return _table[cp].first;
}

uint8 CChsFont::GetColumn(uint32 cp) const
{
	return _table[cp].second;
}
