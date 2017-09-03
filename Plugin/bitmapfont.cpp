#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "cjk_fonts.h"
#include "byte_pattern.h"
#include "hook_variables.h"

namespace BitmapFont
{
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
        void operator()(injector::reg_pack *regs) const
        {

        }
    };

    int __fastcall GetWidthOfString(CBitmapFont * pFont, int edx, const char * text, const int length, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        char tag[128];
        float vTempWidth = 0.0f;
        int nWidth = 0;

        hook_context.cjk_font = CSingleton<CJKFontManager>::Instance().GetFont(pFont->GetFontPath());

        int real_length = length;

        if (length < 0)
        {
            real_length = strlen(text);
        }

        hook_context.wide_text.clear();

        utf8::unchecked::utf8to32(text, text + real_length, back_inserter(hook_context.wide_text));

        for (auto strit = hook_context.wide_text.begin(); strit < hook_context.wide_text.end(); ++strit)
        {
            uint32_t unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA4 || unicode == 0xA7))
            {
                switch (unicode)
                {
                case 0x40:
                    strit += 3;
                    vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    break;

                case 0x7B:
                    strit += 2;
                    vTempWidth += fIconWidth;
                    break;

                case 0xA3:
                    ++strit;

                    {
                        size_t tag_len = 0;
                        uint32_t tag_char = *strit;

                        while (Functions::IsTextIconChar(tag_char) && (tag_len < 127))
                        {
                            tag[tag_len] = tag_char;
                            ++tag_len;
                            ++strit;
                            tag_char = *strit;
                        }

                        tag[tag_len] = 0;
                    }

                    vTempWidth += injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, tag);
                    break;

                case 0xA4:
                    vTempWidth += fIconWidth;
                    break;

                case 0xA7:
                    ++strit;
                    break;

                default:
                    break;
                }
            }
            else
            {
                const EU4CharacterValues *pValues;

                if (Functions::IsNativeChar(unicode))
                {
                    pValues = pFont->GetLatin1Value(unicode);
                }
                else
                {
                    pValues = &hook_context.cjk_font->GetValue(unicode)->EU4Values;
                }

                if (pValues == nullptr)
                {
                    if (unicode == 0xA)
                    {
                        nWidth = max(nWidth, vTempWidth);
                        vTempWidth = 0.0f;
                    }
                }
                else
                {
                    vTempWidth += pValues->xadvance * *pFont->field<float, 0x428>();

                    if (pValues->kerning)
                    {
                        if (strit < hook_context.wide_text.end() - 1)
                        {
                            uint32_t next = *(strit + 1);

                            if (Functions::IsNativeChar(unicode) && Functions::IsNativeChar(next))
                            {
                                CBitmapFontCharacterSet *pSet = pFont->GetLatin1CharacterSet();
                                float fKerning;

                                __asm
                                {
                                    push next;
                                    push unicode;
                                    mov ecx, pSet;
                                    call game_meta.pfCbitmapFontCharacterSet_GetKerning;
                                    movss fKerning, xmm0;
                                }

                                vTempWidth += fKerning;
                            }
                            else
                            {
                                vTempWidth += hook_context.cjk_font->GetKerning(unicode, next);
                            }
                        }
                    }

                    //空格及中文间断
                    if (pValues->h == 0 || !Functions::IsNativeChar(unicode))
                    {
                        nWidth = max(nWidth, vTempWidth);
                    }
                }
            }
        }

        return max(nWidth, vTempWidth);
    }

    void InitAndPatch()
    {
        injector::MakeJMP(g_pattern.find_pattern("81 EC 8C 00 00 00 53 8B 5D 0C").get(0).address(-6), GetWidthOfString);
    }
}
