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

	std::wstring dds_path;

public:	
	void init(HMODULE hself);
};
