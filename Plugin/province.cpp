#include "province.h"
#include "stdinc.h"
#include "bitmapfont.h"

struct InlineBase
{
    virtual void operator()(injector::reg_pack &regs) const = 0;
};

//CGenerateNamesWork_AddNameArea
//GetCharInfo
//PATTERN
//NOPLENGTH
//ADDRESS
struct CGenerateNamesWork_AddNameArea_GetCharInfo
{
    void operator()(injector::reg_pack &regs) const
    {

    }
};

//NAME
//USAGE
//PATTERN
//NOPLENGTH
//ADDRESS
struct NAME_USAGE
{
    void operator()(injector::reg_pack &regs) const
    {

    }
};

namespace Province
{
    void InitAndPatch()
    {

    }
}

