#include "plugin.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		byte_pattern::start_log("eu4chs");
		Plugin::InitAndPatch();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		byte_pattern::shutdown_log();
	}

	return TRUE;
}
