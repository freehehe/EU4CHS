#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "cjk_font.h"

namespace BitmapFont
{
    static const float fIconWidth = 8.0f;

    static CJKFont *coop_font;
    static uint32 code_point;
    static uint32 next_code_point;
    static std::ptrdiff_t cp_len;
    static void *ret_addr;

    //1099880

    __declspec(naked) void CBitmapFont_RenderToScreen_OFF_SIZE()
    {
        __asm
        {
            pop ret_addr;

        }
    }

    struct CBitmapFont_RenderToScreen_OFF_SIZE
    {
        void operator()(injector::reg_pack &regs) const
        {

        }
    };

    void InitAndPatch()
    {

    }
}
