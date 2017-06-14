#include "stdinc.h"
#include "plugin.h"
#include "function.h"
#include "bitmapfont.h"
#include "byte_pattern.h"
#include "../include/injector/hooking.hpp"
#include "../include/injector/assembly.hpp"
#include <cstring>

void plugin::init(HMODULE hself)
{
	wchar_t module_path[512];

	GetModuleFileNameW(hself, module_path, 512);

	std::wcscpy(std::wcsrchr(module_path, L'.'), L"\\font.dds");

	this->dds_path = module_path;

	this->patch();
}


void plugin::patch() const
{
	void *patch_pos;
}
