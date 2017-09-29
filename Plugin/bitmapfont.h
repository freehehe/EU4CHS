#pragma once
#include "eu4.h"
#include "std_string.h"

class CJKFont;

struct CBitmapCharacterSet :IncompleteClass
{
    EU4CharInfo *GetLatin1Value(uint32_t cp)
    {
        if (cp == 0x3 || cp == 0x4 || cp == 0x7)
        {
            cp += 0xA0;
        }

        return field<EU4CharInfo *, 0>()[cp];
    }

    CJKFont *GetCJKFont()
    {
        return reinterpret_cast<CJKFont *>(GetLatin1Value(0));
    }

    float GetScale()
    {
        return get_field<float, 0x428>();
    }
};

#define CHARSET_OFF 0xB4
struct CBitmapFont :IncompleteClass
{
    CBitmapCharacterSet *GetLatin1CharacterSet()
    {
        return field<CBitmapCharacterSet, CHARSET_OFF>();
    }

    const CString *GetFontPath()
    {
        return field<const CString, 0x9C>();
    }

    const EU4CharInfo *GetLatin1Value(uint32_t cp)
    {
        return GetLatin1CharacterSet()->GetLatin1Value(cp);
    }

    int GetTextureWidth()
    {
        return get_field<int, 0x4E8>();
    }

    int GetTextureHeight()
    {
        return get_field<int, 0x4EC>();
    }
};

namespace BitmapFont
{
    void InitAndPatch();
}
