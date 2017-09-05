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

    //预处理字符串，插入换行符
    struct CBitmapFont_RenderToScreen_0x690_13 //1098CA0
    {
        void operator()(injector::reg_pack *regs) const
        {
            char *pSrc = game_meta.pOriginalText + regs->edi;
            char *pDst = game_meta.pWord + regs->esi;

            hook_context.cjkFont = CSingleton<CJKFontManager>::Instance().GetFont((*(CBitmapFont **)(regs->ebp - 0x10))->GetFontPath());
            hook_context.unicodeLength = utf8::internal::sequence_length(pSrc);
            hook_context.unicode = utf8::unchecked::next(pSrc);
            regs->eax = hook_context.unicode;
            utf8::append(hook_context.unicode, pDst);

            regs->edi += (hook_context.unicodeLength - 1);
            regs->esi += hook_context.unicodeLength;

            //SpecialChars全部位于ASCII范围后，不用跳过SpecialChar判断
            //TXT里的A3A4A7怎么办？？读的时候转换啊？？或者渲染时转换？？
        }
    };

    struct CBitmapFont_RenderToScreen_0x85B_9 //1098E6B
    {
        void operator()(injector::reg_pack *regs) const
        {
            if (Functions::IsNativeChar(hook_context.unicode))
            {
                regs->ecx = (uint32_t)((*(CBitmapFontCharacterSet **)(regs->ebp - 0x34))->GetLatin1Value(hook_context.unicode));
            }
            else
            {
                regs->ecx = (uint32_t)(&hook_context.cjkFont->GetValue(hook_context.unicode)->EU4Values);
            }
        }
    };

    __declspec(naked) void CBitmapFont_RenderToScreen_OFF_SIZE()
    {
        __asm
        {
            pop hook_context.ret_addr;

            jmp hook_context.ret_addr;
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

        hook_context.cjkFont = CSingleton<CJKFontManager>::Instance().GetFont(pFont->GetFontPath());

        int real_length = length;

        if (length < 0)
        {
            real_length = strlen(text);
        }

        hook_context.wideText.clear();

        utf8::unchecked::utf8to32(text, text + real_length, back_inserter(hook_context.wideText));

        for (auto strit = hook_context.wideText.begin(); strit < hook_context.wideText.end(); ++strit)
        {
            uint32_t unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x4 || unicode == 0x7))
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

                case 0x3:
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

                case 0x4:
                    vTempWidth += fIconWidth;
                    break;

                case 0x7:
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
                    pValues = &hook_context.cjkFont->GetValue(unicode)->EU4Values;
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
                        if (strit < hook_context.wideText.end() - 1)
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
                                    call game_meta.pfCBitmapFontCharacterSet_GetKerning;
                                    movss fKerning, xmm0;
                                }

                                vTempWidth += fKerning;
                            }
                            else if (!Functions::IsNativeChar(unicode))
                            {
                                vTempWidth += hook_context.cjkFont->GetKerning(unicode, next);
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
        injector::MakeJMP(g_pattern.find_pattern("81 EC 8C 00 00 00 53 8B 5D 0C").get(0).integer(-6), GetWidthOfString);

        //---------------------字符串解析部分--------------------------------
        g_pattern.find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
        injector::MakeInline<CBitmapFont_RenderToScreen_0x690_13>(g_pattern.get(0).integer(), g_pattern.get(0).integer(13));

        //---------------------------0xA3 0xA4 0xA7-------------------------------
        g_pattern.find_pattern("80 3C 38 A?"); //cmp byte ptr [eax + edi], 0xA?
        g_pattern.for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(3);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        g_pattern.find_pattern("3C A? 75"); //cmp al, 0xA?; jnz short
        g_pattern.for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(1);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }

        });

        g_pattern.find_pattern("3C A? 0F 85"); //cmp al, 0xA?; jnz long
        g_pattern.for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(1);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //漏网之鱼

        //----------------------------WriteVariable 0xA7----------------------------------------
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("C6 06 A7").get(0).integer(2), 7, true);
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("66 C7 06 A7 21").get(0).integer(3), 7, true);
    }
}
