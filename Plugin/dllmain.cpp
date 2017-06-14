#include "plugin.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		singleton<plugin>::instance().init(module);
	}

	return TRUE;
}
