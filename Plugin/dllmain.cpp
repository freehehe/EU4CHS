#include "plugin.h"
#include "stdinc.h"
#include <vector>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		singleton<Plugin>::instance().init(module);
	}

	return TRUE;
}
