#include "stdinc.h"
#include "plugin.h"
#include "functions.h"
#include "bitmapfont.h"
#include "byte_pattern.h"
#include "../include/injector/hooking.hpp"
#include "../include/injector/assembly.hpp"
#include "../include/injector/calling.hpp"
#include "../include/utf8cpp/utf8.h"
#include <cstring>
#include "eu4.h"

void plugin::init(HMODULE hself)
{
	char module_path[512];
	char filename[512];

	GetModuleFileNameA(hself, module_path, 512);

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dds");
	this->dds_path = filename;

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dat");
	this->dat_path = filename;

	this->patch();
}

const char *plugin::data_path() const
{
	return this->dat_path.c_str();
}

const char *plugin::texture_path() const
{
	return this->dds_path.c_str();
}

void plugin::patch() const
{

}
