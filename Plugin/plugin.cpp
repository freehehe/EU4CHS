#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "table.h"
#include "vfs.h"
#include "byte_pattern.h"

static std::string font_path;
static std::string mapfont_path;
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
	font_path = filename;

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\mapfont.dds");
	mapfont_path = filename;

	std::strcpy(filename, module_path);
	std::strcpy(std::strrchr(filename, '\\'), "\\eu4chs\\font.dat");
	table_path = filename;

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

const char *Plugin::GetFontPath()
{
	return font_path.c_str();
}

const char *Plugin::GetMapFontPath()
{
	return mapfont_path.c_str();
}


const char *Plugin::GetTablePath()
{
	return table_path.c_str();
}

void Plugin::Patch()
{
	CGlobalFunctions::Patch();
	VFS::Patch();

	//贴图大小检测
	injector::WriteMemory<uint32_t>(g_pattern.set_module(pattern_default_module).set_pattern("81 FE 00 00 00 01").force_search().get(0).pointer(2), 0x7FFFFFFFu, true);
}
