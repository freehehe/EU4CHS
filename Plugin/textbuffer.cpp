#include "textbuffer.h"

namespace TextBuffer
{
    CString * CTextBuffer::GetString()
    {
        return field<CString, 0x1C>();
    }

    BitmapFont::CBitmapFont * CTextBuffer::GetFont()
    {
        return get_field<BitmapFont::CBitmapFont *, 0x70>();
    }

    bool CTextBuffer::UseSpecialChar()
    {
        return get_field<bool, 0xAC>();
    }

    //
    //
    struct CTextBuffer_
    {
        void operator()(injector::reg_pack &regs) const
        {

        }
    };

    void InitAndPatch()
    {

    }
}
