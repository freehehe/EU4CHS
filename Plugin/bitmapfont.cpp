#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "NonLatinFont.h"

namespace BitmapFont
{
    static const float fIconWidth = 8.0f;

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

                    if (pvalue->kerning && (it + 1) != wtext.end())
                    {
                        uint32 nextcp = *(it + 1);

                        CBitmapFontCharacterSet *pset = pFont->GetCharacterSet();

                        vTempWidth += GetKerning(pset, 0, cp, nextcp);
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

    float __fastcall GetKerning(CBitmapFontCharacterSet *pSet, int edx, uint32 first, uint32 second)
    {
        float result = 0.0f;

        if (Functions::IsNativeCharacter(first) && Functions::IsNativeCharacter(second))
        {
            __asm
            {
                push second;
                push first;
                mov ecx, pSet;
                //call game_meta.pfCBitmapFontCharacterSet_GetKerning;
                movss result, xmm0;
            }
        }

        __asm movss xmm0, result;

        return result;
    }

    static NonLatinFont *coop_font;
    static uint32 code_point;
    static uint32 next_code_point;
    static std::ptrdiff_t cp_len;
    static void *ret_addr;

    //1099880

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

    void InitAndPatch()
    {

    }
}
