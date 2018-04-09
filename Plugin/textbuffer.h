#pragma once
#include "stdinc.h"
#include "eu4.h"
#include "bitmapfont.h"

namespace TextBuffer
{
    typedef std::pair<CString, bool> CRow;

    class CTextBuffer :IncompleteClass
    {
        //0x1C: string
        //0x70: pFont
        //0xAC: use special char
    public:
        CString *GetString();
        BitmapFont::CBitmapFont *GetFont();
    };

    void InitAndPatch();
}
