#include "plugin.h"
#include <string>
#include <cstring>

static std::string texture_path;
static std::string table_path;

void Plugin::Init(HMODULE hself)
{
	char module_path[512];
	char filename[512];

	GetModuleFileNameA(hself, module_path, 512);

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dds");
	texture_path = filename;

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dat");
	table_path = filename;

	Patch();
}

const char *Plugin::GetTexturePath()
{
	return texture_path.c_str();
}

const char *Plugin::GetTablePath()
{
	return table_path.c_str();
}

void Plugin::Patch()
{


}
