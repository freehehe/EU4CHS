#include "province.h"
#include "misc.h"
#include "eu4.h"
#include "bitmapfont.h"
#include "byte_pattern.h"

namespace Province
{
    using namespace std;

    //E8 ? ? ? ? 8B 45 AC 8D 55 BC
    //5
    //"ABC" --> "A B C"
    struct CGenerateNamesWorkAddNamesArea_InsertSpace
    {
        void operator()(injector::reg_pack &regs) const
        {
            static vector<uint32_t> wbuffer;
            static string cbuffer;

            wbuffer.clear();
            cbuffer.clear();

            regs.esp.i += 8;
            const CString *pSource = (CString *)(regs.ebp.i - 0xC0);
            CString *pDest = regs.ecx;

            const char *it = pSource->c_str();
            const char *end = pSource->c_str() + strlen(pSource->c_str());

            while (it != end)
            {
                wbuffer.push_back(eu4utf8::next(it, end));
                wbuffer.push_back(' ');
            }
            wbuffer.pop_back();

            eu4utf8::utf32to8(wbuffer.begin(), wbuffer.end(), back_inserter(cbuffer));

            Misc::assign_string(pDest, cbuffer.c_str());
        }
    };

    //0F B6 04 08 8B 04 82
    //7
    struct CGenerateNamesWorkAddNamesArea_CountDrawables
    {
        void operator()(injector::reg_pack &regs) const
        {
            BitmapFont::CBitmapCharacterSet *pSet = regs.edx;
            Misc::context.useSpecialChars = false;

            Misc::GetTwoUnicode(regs.eax, regs.ecx.i);

            regs.eax.p = pSet->GetCharacterValue(Misc::context.unicode);
            regs.ecx.i += (Misc::context.unicodeLength - 1);
        }
    };

    //8B 79 10 83 FF 02
    //6
    struct CurveText_CalcExtraWidth
    {
        void operator()(injector::reg_pack &regs) const
        {
            const CString *pString = regs.ecx;

            regs.edi.i = eu4utf8::distance(pString->c_str(), pString->c_str() + strlen(pString->c_str()));

            regs.ef.zero_flag = (regs.edi.i == 2);
        }
    };

    //8B 04 82 89 45 9C 85 C0
    //6
    struct CurveText_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            BitmapFont::CBitmapCharacterSet *pSet = regs.edx;
            uint32_t index = *(uint32_t *)(regs.ebp.i - 0x1C);

            Misc::context.useSpecialChars = false;

            Misc::GetTwoUnicode(regs.ecx, index);

            *(uint32_t *)(regs.ebp.i - 0x1C) += (Misc::context.unicodeLength - 1);

            regs.eax.p = pSet->GetCharacterValue(Misc::context.unicode);
            *(void **)(regs.ebp.i - 0x64) = regs.eax.p;
        }
    };

    //3C 27 74 0D 3C 2E
    //6
    struct CurveText_CheckDots
    {
        void operator()(injector::reg_pack &regs) const
        {
            regs.ef.zero_flag = (Misc::context.unicode == 0x27 || Misc::context.unicode == 0x2E);
        }
    };

    //0F B6 04 08 8B 84 86 B4 00 00 00
    //11
    struct CCountryNameCollectionNudgedNames_CountDrawables
    {
        void operator()(injector::reg_pack &regs) const
        {
            BitmapFont::CBitmapFont *pFont = regs.esi;
            Misc::context.useSpecialChars = false;

            Misc::GetTwoUnicode(regs.eax, regs.ecx.i);

            regs.eax.p = pFont->GetCharacterSet()->GetCharacterValue(Misc::context.unicode);
            regs.ecx.i += (Misc::context.unicodeLength - 1);
        }
    };

    void InitAndPatch()
    {
        //AddNameArea
        g_pattern.find_pattern("E8 ? ? ? ? 8B 45 AC 8D 55 BC");
        if (g_pattern.has_size(1))
            injector::MakeInline<CGenerateNamesWorkAddNamesArea_InsertSpace>(g_pattern.get_first().address());

        g_pattern.find_pattern("0F B6 04 08 8B 04 82");
        if (g_pattern.has_size(1))
            injector::MakeInline<CGenerateNamesWorkAddNamesArea_CountDrawables>(g_pattern.get_first().address(), g_pattern.get_first().address(7));

        //CurveText
        g_pattern.find_pattern("8B 79 10 83 FF 02");
        if (g_pattern.has_size(1))
            injector::MakeInline<CurveText_CalcExtraWidth>(g_pattern.get_first().address(), g_pattern.get_first().address(6));

        g_pattern.find_pattern("8B 04 82 89 45 9C 85 C0");
        if (g_pattern.has_size(1))
            injector::MakeInline<CurveText_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(6));
    
        g_pattern.find_pattern("3C 27 74 0D 3C 2E");
        if (g_pattern.has_size(1))
            injector::MakeInline<CurveText_CheckDots>(g_pattern.get_first().address(), g_pattern.get_first().address(6));

        //AddNudgedNames
        g_pattern.find_pattern("0F B6 04 08 8B 84 86 B4 00 00 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CCountryNameCollectionNudgedNames_CountDrawables>(g_pattern.get_first().address(), g_pattern.get_first().address(11));
    }
}
