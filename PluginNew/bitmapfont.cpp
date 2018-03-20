#include "bitmapfont.h"
#include "byte_pattern.h"

namespace BitmapFont
{
    static char *pOriginalText;
    static char *pWord;
    static char *pText;

    void InitAndPatch()
    {
        g_pattern.find_pattern("68 00 0C 00 00 50 68");
        if (g_pattern.has_size(1))
            pOriginalText = *g_pattern.get_first().pointer<char *>(7);

        g_pattern.find_pattern("68 3C 0C 00 00 6A 00 68");
        if (g_pattern.has_size(1))
            pText = *g_pattern.get_first().pointer<char *>(8);

        g_pattern.find_pattern("68 00 01 00 00 6A 00 68");
        if (g_pattern.has_size(1))
            pWord = *g_pattern.get_first().pointer<char *>(8);


    }
}
