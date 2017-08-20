#include "stdinc.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "NonLatinFont.h"

namespace BitmapFont
{
    static const float fIconWidth = 8.0f;

    static NonLatinFont normalFont, mapFont;

    int __fastcall GetWidthOfString(CBitmapFont *pFont, int edx, const char *text, const int length, bool bUseSpecialChars)
    {
        static std::vector<uint32> wtext;

        float vTempWidth = 0.0f;
        int nWidth = 0;
        char tag[128];

        int real_length = length < 0 ? std::strlen(text) : length;

        wtext.clear();
        utf8::unchecked::utf8to32(text, text + real_length, std::back_inserter(wtext));

        auto it = wtext.begin();

        while (it != wtext.end())
        {
            uint32 cp = *it;

            EU4CharacterValues *pvalue = pFont->GetValueByCodePoint(cp);

            if (bUseSpecialChars && (cp == 0xA7 || cp == 0xA3 || cp == 0xA4 || cp == 0x40 || cp == 0x7B))
            {
                switch (cp)
                {
                case 0xA7:
                    ++it;
                    break;

                case 0x40:
                    it += 3;
                    vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    break;

                case 0xA3:
                {
                    std::fill(std::begin(tag), std::end(tag), 0);

                    ++it;

                    auto len = std::count_if(it, wtext.end(), Functions::IsTextIconChar);

                    utf8::unchecked::utf32to8(it, it + len, std::begin(tag));

                    it += len;

                    vTempWidth += injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, tag);
                }
                break;

                case 0xA4:
                    vTempWidth += fIconWidth;
                    break;

                case 0x7B:
                    it += 2;
                    vTempWidth += fIconWidth;
                    break;

                default:
                    break;
                }
            }
            else
            {
                EU4CharacterValues *pvalue = pFont->GetValueByCodePoint(cp);

                if (pvalue)
                {
                    vTempWidth += pvalue->xadvance * *pFont->GetCharacterSet()->GetScaleX();

                    if (pvalue->kerning && (it + 1) != wtext.end() && Functions::IsNativeCharacter(*(it + 1)))
                    {
                        uint32 nextcp = *(it + 1);
                        CBitmapFontCharacterSet *pset = pFont->GetCharacterSet();
                        float fkerning;

                        _asm
                        {
                            push nextcp;
                            push cp;
                            mov ecx, pset;
                            call game_meta.pfCBitmapFontCharacterSet_GetKerning;
                            movss fkerning, xmm0;
                        }

                        vTempWidth += fkerning;
                    }

                    if (pvalue->h == 0 || !Functions::IsNativeCharacter(cp))
                    {
                        nWidth = std::max(vTempWidth, (float)nWidth);
                    }
                }
                else
                {
                    if (cp == 0xA)
                    {
                        nWidth = std::max(vTempWidth, (float)nWidth);
                        vTempWidth = 0.0f;
                    }
                }
            }

            ++it;
        }

        return std::max(vTempWidth, (float)nWidth);
    }
}

static uint32 code_point;
static uint32 next_code_point;
static std::ptrdiff_t cp_len;
static void *ret_addr;

struct CBitmapFont_RenderToScreen_0x690_13
{
    void operator()(injector::reg_pack &regs) const
    {
        char *source = game_meta.pOriginalText + regs.edi;
        char *dest = game_meta.pWord + regs.esi;

        cp_len = utf8::internal::sequence_length(source);

        code_point = utf8::unchecked::next(source);
        next_code_point = utf8::unchecked::next(source);

        regs.eax = code_point;

        utf8::unchecked::append(code_point, dest);

        regs.edi += (cp_len - 1);
        regs.esi += cp_len;

        if (!Functions::IsNativeCharacter(code_point))
        {
            regs.ecx = 0;
        }
    }
};

struct CBitmapFont_RenderToScreen_0x85B_9
{
    void operator()(injector::reg_pack &regs) const
    {
        CBitmapFont *_this = *(CBitmapFont **)(regs.ebp - 0x10);

        regs.ecx = (uint32)_this->GetValueByCodePoint(code_point);
    }
};

__declspec(naked) void CBitmapFont_RenderToScreen_0x8CE_20()
{
    __asm
    {
        pop ret_addr;
        cmp next_code_point, 0xFF;
        ja no_kerning;
        push next_code_point;
        push code_point;
        mov ecx, [ebp - 0x34];
        call game_meta.pfCBitmapFontCharacterSet_GetKerning;
        jmp j_ret;

    no_kerning:
        xorps xmm0, xmm0;

    j_ret:
        jmp ret_addr;
    }
}

__declspec(naked) void CBitmapFont_RenderToScreen_0x8FA_11()
{
    __asm
    {
        pop ret_addr;
        cmp code_point, 0xFF;
        ja j_break;
        cmp[ecx]EU4CharacterValues.h, 0;
        jmp ret_addr;

    j_break:
        add ret_addr, 6;
        jmp ret_addr;
    }
}



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

void BitmapFont::Patch()
{

}
