#pragma once
#include "stdinc.h"

namespace TextBuffer
{
    //
    //
    struct CTextBuffer
    {
        void operator()(injector::reg_pack &regs) const
        {

        }
    };

    class CTextBuffer :IncompleteClass
    {

    };

    void InitAndPatch();
}
