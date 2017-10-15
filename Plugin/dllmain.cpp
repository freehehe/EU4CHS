#include "plugin.h"
#include "eu4.h"
#include "functions.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_plugin.InitAndPatch(module);
    }

    return TRUE;
}
