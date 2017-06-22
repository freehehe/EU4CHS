#include "stdinc.h"
#include "plugin.h"
#include "functions.h"
#include "bitmapfont.h"
#include "byte_pattern.h"
#include "../include/injector/hooking.hpp"
#include "../include/injector/assembly.hpp"
#include "../include/injector/calling.hpp"
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
	void *patch_pointer;
	std::uintptr_t patch_address;

	g_pattern.set_pattern("55 8B EC 6A FF 64 A1 00 00 00 00 68 ? ? ? ? 50 64 89 25 00 00 00 00 81 EC B0 00 00 00 53 56 8B F1 8B DA 57 80 3E 00").force_search();

	if (!g_pattern.empty())
	{
		patch_pointer = g_pattern.get(0).pointer();
		injector::MakeJMP(patch_pointer, ConvertUTF8ToLatin1);
	}



}

struct LoadTextureHook
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};

struct UnloadTextureHook
{
	void operator()(injector::reg_pack &regs) const
	{

	}
};
