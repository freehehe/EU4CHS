#include "plugin.h"
#include "positions.h"
#include <fstream>
#include <iterator>

char_positions::char_positions()
{
	this->data = std::make_unique<std::array<position, 0xFFFF>>();
	this->data->fill(position{ 63,63 });
	this->read_table(singleton<plugin>::instance().data_path());
}

void char_positions::read_table(const char *filename)
{
	std::ifstream ifs(filename, std::ios::binary);

	if (ifs)
	{
		std::copy_n(std::istream_iterator<position>(ifs), this->data->size(), this->data->begin());
	}
}

unsigned char char_positions::find_row(uint32 cp) const
{
	return this->data->at(cp).row;
}

unsigned char char_positions::find_column(uint32 cp) const
{
	return this->data->at(cp).column;
}
