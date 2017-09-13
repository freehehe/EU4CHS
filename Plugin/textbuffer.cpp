#include "stdinc.h"
#include "textbuffer.h"

namespace TextBuffer
{
    void Patch()
    {
        //CTextBuffer::Write
        //CString CTextBuffer::field_88 关键字
        injector::
        injector::MakeRET(g_pattern.find_first("8B 4D B8 32 C0").integer(), 4);
    }
}
