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
    struct CBitmapFont_RenderToScreen_0x690_13 //1098CA0
    {
        void operator()(injector::reg_pack *regs) const
        {
            char *pSrc = game_meta.pOriginalText + regs->edi;
            char *pDst = game_meta.pWord + regs->esi;

            hook_context.cjkFont = CSingleton<CJKFontManager>::Instance().GetFont((*(CBitmapFont **)(regs->ebp - 0x10))->GetFontPath());
            hook_context.unicodeLength = utf8::internal::sequence_length(pSrc);

            hook_context.unicode = utf8::unchecked::next(pSrc);
            hook_context.nextUnicode = utf8::unchecked::next(pSrc);

            regs->eax = hook_context.unicode;
            utf8::append(hook_context.unicode, pDst);

            regs->edi += (hook_context.unicodeLength - 1);
            regs->esi += hook_context.unicodeLength;
            *(uint32_t *)(regs->ebp - 0x30) = regs->edi;
            regs->edx = *(uint32_t *)(regs->ebp - 0x10);

            //因为只比较低8位，还是得跳过。。。
            if (!Functions::IsNativeChar(hook_context.unicode))
            {
                regs->ecx = 0;
            }
            else
            {
                regs->ecx = *(uint8_t *)(regs->ebp + 0x3C);
            }
        }
    };

    struct CBitmapFont_RenderToScreen_0x85B_9  //1098E6B
    {
        void operator()(injector::reg_pack *regs) const
        {
            regs->edx = *(uint32_t *)(regs->ebp - 0x34);

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

    __declspec(naked) void CBitmapFont_RenderToScreen_0x8CE_20() //1098EDE
    {
        __asm
        {
            pop hook_context.ret_addr;

            cmp hook_context.nextUnicode, 0xFF;
            ja skip;
            push hook_context.nextUnicode;
            push hook_context.unicode;
            mov ecx, [ebp - 0x34];
            call game_meta.pfCBitmapFontCharacterSet_GetKerning;
            jmp end;

        skip:
            xorps xmm0, xmm0;

        end:
            jmp hook_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_RenderToScreen_0x8FA() //1098F0A
    {
        __asm
        {
            //al为0则处理间断
            pop hook_context.ret_addr;

            xor al, al;
            mov dl, 1;

            //高度不为0 && 前字节<=255 && 后字节<=255
            cmp [ecx]EU4CharacterValues.h, 0; //高度为0
            setnz al;
            and dl, al;

            cmp hook_context.unicode, 0xFF; //大于255
            setbe al;
            and dl, al;

            //cmp hook_context.nextUnicode, 0xFF; //大于255
            //setbe al;
            //and dl, al;

            test dl, dl;
            jmp hook_context.ret_addr;
        }
    }

    struct CBitmapFont_RenderToScreen_0x1797_12 //1099DA7
    {
        void operator()(injector::reg_pack *regs) const
        {
            char *pSrc = game_meta.pText + regs->edi;

            hook_context.unicodeLength = utf8::internal::sequence_length(pSrc);
            hook_context.unicode = utf8::unchecked::next(pSrc);
            hook_context.nextUnicode = utf8::unchecked::next(pSrc);
            regs->eax = hook_context.unicode;
            regs->ecx = hook_context.unicode;
            regs->edi += (hook_context.unicodeLength - 1);

            if (Functions::IsNativeChar(hook_context.unicode))
            {
                regs->esi = (uint32_t)((*(CBitmapFontCharacterSet **)(regs->ebp - 0x34))->GetLatin1Value(hook_context.unicode));
            }
            else
            {
                regs->esi = (uint32_t)(&hook_context.cjkFont->GetValue(hook_context.unicode)->EU4Values);
            }

            regs->edx = *(uint32_t *)(regs->ebp - 0x34);
        }
    };

    __declspec(naked) void CBitmapFont_RenderToScreen_0x2044_19() //109A654
    {
        __asm
        {
            pop hook_context.ret_addr;

            push ebx;
            mov ebx, edx;

            cmp hook_context.unicode, 0xFF;
            jg nonLatin;
            jmp latin;

        nonLatin:
            sub dword ptr[ebp - 0x40], 6;
            mov eax, game_meta.pBitmapVertices;
            lea eax, [eax + ebx * 4];
            push eax;
            push hook_context.unicode;
            push[ebp - 0x10];
            mov ecx, hook_context.cjkFont;
            call CJKFont::AddVerticesDX9;

        latin:
            mov ecx, [ebp - 0x40];
            mov eax, 0x2AAAAAAB;
            imul ecx;
            mov eax, ebx;
            shr eax, 0x1F;
            add eax, ebx;
            cmp eax, 0xC2;

        end:
            pop ebx;
            jmp hook_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_RenderToScreen_0x20E4_27() //109A6F4
    {
        __asm
        {
            pop hook_context.ret_addr;
            cmp hook_context.nextUnicode, 0xFF;
            jg noKerning;

            push hook_context.nextUnicode;
            push hook_context.unicode;
            lea ecx, [esi + 0xB4];
            call game_meta.pfCBitmapFontCharacterSet_GetKerning;

            jmp end;

        noKerning:
            xorps xmm0, xmm0;

        end:
            jmp hook_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_GeiHeightOfString_OFF_SIZE()
    {
        __asm
        {
            pop hook_context.ret_addr;

            jmp hook_context.ret_addr;
        }
    }

    struct CBitmapFont_GeiHeightOfString_OFF_SIZE
    {
        void operator()(injector::reg_pack *regs) const
        {

        }
    };

    //获取字符串的宽度，用于计算背景宽度，不限最大宽度
    static int __fastcall GetWidthOfString(CBitmapFont * pFont, int, const char * text, const int length, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        char Tag[128];
        float vTempWidth = 0.0f; //当前行宽度
        int nWidth = 0; //最大行宽度

        hook_context.cjkFont = CSingleton<CJKFontManager>::Instance().GetFont(pFont->GetFontPath());

        int nSize = length;

        if (length < 0)
        {
            nSize = strlen(text);
        }

        hook_context.wideText.clear();

        utf8::unchecked::utf8to32(text, text + nSize, back_inserter(hook_context.wideText));

        for (auto strit = hook_context.wideText.begin(); strit < hook_context.wideText.end(); ++strit)
        {
            uint32_t unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x4 || unicode == 0x7))
            {
                if (unicode == 0x40)
                {
                    strit += 3;
                    vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                }
                else if (unicode == 0x7B)
                {
                    strit += 2;
                    vTempWidth += fIconWidth;
                }
                else if (unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(strit[index]) && (index < 127) && (strit < hook_context.wideText.end()))
                    {
                        Tag[index] = *strit;
                        ++index;
                        ++strit;
                    }

                    Tag[index] = 0;

                    vTempWidth += injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, Tag);
                }
                else if (unicode == 0x4)
                {
                    vTempWidth += fIconWidth;
                }
                else if (unicode == 0x7)
                {
                    ++strit;
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
                    vTempWidth += pValues->xadvance * *pFont->GetLatin1CharacterSet()->GetScaleX();

                    if (pValues->kerning && (strit < (hook_context.wideText.end() - 1)))
                    {
                        uint32_t next = *(strit + 1);

                        if (Functions::IsNativeChar(next))
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
                    }

                    if (pValues->h == 0 || !Functions::IsNativeChar(unicode))
                    {
                        nWidth = max(nWidth, vTempWidth);
                    }
                }
            }
        }

        return max(nWidth, vTempWidth);
    }

    static int __fastcall GetHeightOfString(CBitmapFont * pFont, int, const CString *text, int nMaxWidth, int nMaxHeight, const CVector2<int> *BorderSize, bool bUseSpecialChars)
    {
        char Tag[128];
        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;
        float vCharacterHeight
        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        nMaxWidth -= 2 * BorderSize->x;

        std::string_view text_view(text->c_str());
    }

    void InitAndPatch()
    {
        injector::MakeJMP(g_pattern.find_pattern("81 EC 8C 00 00 00 53 8B 5D 0C").get(0).integer(-6), GetWidthOfString);

        //---------------------字符串解析部分--------------------------------
        g_pattern.find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
        injector::MakeInline<CBitmapFont_RenderToScreen_0x690_13>(g_pattern.get(0).integer(), g_pattern.get(0).integer(13));

        g_pattern.find_pattern("8B 55 CC 0F B6 C0 8B 0C 82");
        injector::MakeInline<CBitmapFont_RenderToScreen_0x85B_9>(g_pattern.get(0).integer(), g_pattern.get(0).integer(9));

        g_pattern.find_pattern("8A 04 38 FF 75 A8");
        injector::MakeNOP(g_pattern.get(0).integer(), 20);
        injector::MakeCALL(g_pattern.get(0).integer(), CBitmapFont_RenderToScreen_0x8CE_20);


        g_pattern.find_pattern("66 83 79 06 00");
        injector::MakeCALL(g_pattern.get(0).integer(), CBitmapFont_RenderToScreen_0x8FA);
        
        g_pattern.find_pattern("8A 8F ? ? ? ? 8B 55 CC");
        injector::MakeNOP(g_pattern.get(0).integer(0x14), 3); //mov esi, [edx+eax*4]
        injector::MakeInline<CBitmapFont_RenderToScreen_0x1797_12>(g_pattern.get(0).integer(), g_pattern.get(0).integer(12));

        g_pattern.find_pattern("89 04 95 ? ? ? ? B8 AB AA AA 2A");
        injector::MakeNOP(g_pattern.get(0).integer(7), 19);
        injector::MakeCALL(g_pattern.get(0).integer(7), CBitmapFont_RenderToScreen_0x2044_19);

        g_pattern.find_pattern("0F B6 87 ? ? ? ? 8D 8E B4 00 00 00");
        injector::MakeNOP(g_pattern.get(0).integer(), 27);
        injector::MakeCALL(g_pattern.get(0).integer(), CBitmapFont_RenderToScreen_0x20E4_27);

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
