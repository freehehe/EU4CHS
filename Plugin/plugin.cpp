#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "province.h"

void Plugin::InitAndPatch()
{
    Functions::InitAndPatch();
    CBitmapFont::InitAndPatch();
    Province::InitAndPatch();
}
