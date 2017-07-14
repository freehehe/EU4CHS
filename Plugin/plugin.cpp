#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "table.h"
#include "vfs.h"
#include <string>
#include <cstring>

static std::string texture_path;
static std::string table_path;
static HMODULE hasi;
static HMODULE hexe;

void Plugin::Init(HMODULE hself)
{
	char module_path[512];
	char filename[512];

	hexe = GetModuleHandleA(NULL);
	hasi = hself;

	GetModuleFileNameA(hself, module_path, 512);

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dds");
	texture_path = filename;

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dat");
	table_path = filename;

	VFS::EnumerateOurFiles();

	Patch();
}

HMODULE Plugin::GetASIHandle()
{
	return hasi;
}

HMODULE Plugin::GetEXEHandle()
{
	return hexe;
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
	CCharTable::ReadTable(GetTablePath());
	CBitmapFont::Patch();
	CGlobalFunctions::Patch();
	VFS::Patch();
}
