#include "misc.h"
#include "bitmapfont.h"
#include "province.h"
#include "textbuffer.h"
#include "byte_pattern.h"

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        byte_pattern::start_log("eu4chs");

        Misc::InitAndPatch();
        BitmapFont::InitAndPatch();
        Province::InitAndPatch();
        TextBuffer::InitAndPatch();
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        byte_pattern::shutdown_log();
    }

    return TRUE;
}
