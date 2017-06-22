#pragma once
#include <windows.h>
#include "singleton.hpp"
#include <string>

class plugin
{
	friend class singleton<plugin>;

	plugin() = default;
	~plugin() = default;
	plugin &operator==(const plugin &) = delete;
	plugin(const plugin &) = delete;

	void patch() const;

	std::string dds_path;
	std::string dat_path;

	void init(HMODULE hself);

public:	
	const char *texture_path() const;
	const char *data_path() const;
};
