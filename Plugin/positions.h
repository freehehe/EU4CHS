#pragma once
#include "stdinc.h"
#include "singleton.hpp"
#include <memory>
#include <array>

class char_positions
{
	friend class singleton<char_positions>;

	struct position
	{
		unsigned char row;
		unsigned char column;
	};

	std::unique_ptr<std::array<position, 65536>> data;

	char_positions();
	~char_positions() = default;
	char_positions &operator==(const char_positions &) = delete;
	char_positions(const char_positions &) = delete;

public:
	void read_table(const char *filename);

	unsigned char find_row(uint32 cp) const;
	unsigned char find_column(uint32 cp) const;
};
