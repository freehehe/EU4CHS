#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "cjk_fonts.h"
#include "hook_variables.h"

namespace BitmapFont
{
    struct CBitmapFont_RenderToScreen_0x690_13 //1098CA0
    {
        void operator()(injector::reg_pack *regs) const
        {
            char *pSrc = g_game.pOriginalText + regs->edi;
            char *pDst = g_game.pWord + regs->esi;

            g_context.cjkFont = g_Fonts.GetFont((*(CBitmapFont **)(regs->ebp - 0x10))->GetFontPath());
            g_context.unicodeLength = utf8::internal::sequence_length(pSrc);

            g_context.unicode = utf8::unchecked::next(pSrc);
            g_context.nextUnicode = utf8::unchecked::peek_next(pSrc);
            g_context.nextDrawableUnicode = Functions::GetNextDrawableUnicode(pSrc, *(bool *)(regs->ebp + 0x3C));

            regs->eax = g_context.unicode;
            utf8::append(g_context.unicode, pDst);

            regs->edi += (g_context.unicodeLength - 1);
            regs->esi += g_context.unicodeLength;
            *(uint32_t *)(regs->ebp - 0x30) = regs->edi;
            regs->edx = *(uint32_t *)(regs->ebp - 0x10);

            //因为只比较低8位，还是得跳过。。。
            if (!Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ecx = 0;
            }
        }
    };

    struct CBitmapFont_RenderToScreen_0x85B_9  //1098E6B
    {
        void operator()(injector::reg_pack *regs) const
        {
            regs->edx = *(uint32_t *)(regs->ebp - 0x34);

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ecx = (uint32_t)((*(CBitmapCharacterSet **)(regs->ebp - 0x34))->GetLatin1Value(g_context.unicode));
            }
            else
            {
                regs->ecx = (uint32_t)(&g_context.cjkFont->GetValue(g_context.unicode)->EU4Values);
            }
        }
    };

    __declspec(naked) void CBitmapFont_RenderToScreen_0x8CE_20() //1098EDE
    {
        __asm
        {
            pop g_context.ret_addr;

            cmp g_context.nextUnicode, 0xFF;
            ja skip;
            push g_context.nextUnicode;
            push g_context.unicode;
            mov ecx, [ebp - 0x34];
            call g_game.pfCBitmapCharacterSet_GetKerning;
            jmp end;

        skip:
            xorps xmm0, xmm0;

        end:
            jmp g_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_RenderToScreen_0x8FA() //1098F0A
    {
        __asm
        {
            //al为0则处理间断
            pop g_context.ret_addr;

            xor al, al;
            mov dl, 1;

            //高度不为0 && 前字节<=255 && 后字节<=255
            cmp [ecx]EU4CharacterValues.h, 0; //高度为0
            setnz al;
            and dl, al;

            cmp g_context.unicode, 0xFF; //大于255
            setbe al;
            and dl, al;

            //cmp hook_context.nextDrawableUnicode, 0xFF; //大于255
            //setbe al;
            //and dl, al;

            test dl, dl;
            jmp g_context.ret_addr;
        }
    }

    struct CBitmapFont_RenderToScreen_0x1797_12 //1099DA7
    {
        void operator()(injector::reg_pack *regs) const
        {
            char *pSrc = g_game.pText + regs->edi;

            g_context.unicodeLength = utf8::internal::sequence_length(pSrc);
            g_context.unicode = utf8::unchecked::next(pSrc);
            g_context.nextUnicode = utf8::unchecked::peek_next(pSrc);
            g_context.nextDrawableUnicode = Functions::GetNextDrawableUnicode(pSrc, *(bool *)(regs->ebp + 0x3C));

            regs->eax = g_context.unicode;
            regs->ecx = g_context.unicode;
            regs->edi += (g_context.unicodeLength - 1);

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->esi = (uint32_t)((*(CBitmapCharacterSet **)(regs->ebp - 0x34))->GetLatin1Value(g_context.unicode));
            }
            else
            {
                regs->esi = (uint32_t)(&g_context.cjkFont->GetValue(g_context.unicode)->EU4Values);
            }

            regs->edx = *(uint32_t *)(regs->ebp - 0x34);
        }
    };

    __declspec(naked) void CBitmapFont_RenderToScreen_0x2044_19() //109A654
    {
        __asm
        {
            pop g_context.ret_addr;

            push ebx;
            mov ebx, edx;

            cmp g_context.unicode, 0xFF;
            jg nonLatin;
            jmp latin;

        nonLatin:
            sub dword ptr[ebp - 0x40], 6;
            mov eax, g_game.pBitmapVertices;
            lea eax, [eax + ebx * 4];
            push eax;
            push g_context.unicode;
            push [ebp - 0x10];
            mov ecx, g_context.cjkFont;
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
            jmp g_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_RenderToScreen_0x20E4_27() //109A6F4
    {
        __asm
        {
            pop g_context.ret_addr;
            cmp g_context.nextUnicode, 0xFF;
            jg noKerning;

            push g_context.nextUnicode;
            push g_context.unicode;
            lea ecx, [esi + 0xB4];
            call g_game.pfCBitmapCharacterSet_GetKerning;

            jmp end;

        noKerning:
            xorps xmm0, xmm0;

        end:
            jmp g_context.ret_addr;
        }
    }

    __declspec(naked) void CBitmapFont_RenderToTexture_0x_()
    {
        __asm
        {
            pop g_context.ret_addr;

            jmp g_context.ret_addr;
        }
    }

    struct CBitmapFont_RenderToTexture_0x_
    {
        void operator()(injector::reg_pack *regs) const
        {

        }
    };

    static int __fastcall GetWidthOfString(CBitmapFont * pFont, int, const char * text, const int length, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        char Tag[128];
        float vTempWidth = 0.0f; //当前行宽度
        int nWidth = 0; //最大行宽度

        g_context.cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        int nSize = length;

        if (length < 0)
        {
            nSize = strlen(text);
        }

        g_context.wideText.clear();
        utf8::unchecked::utf8to32(text, text + nSize, back_inserter(g_context.wideText));

        for (auto strit = g_context.wideText.begin(); strit < g_context.wideText.end(); ++strit)
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

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < g_context.wideText.end()))
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

                if (Functions::IsLatin1Char(unicode))
                {
                    pValues = pFont->GetLatin1Value(unicode);
                }
                else
                {
                    pValues = &g_context.cjkFont->GetValue(unicode)->EU4Values;
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
                    vTempWidth += pValues->xadvance * pFont->GetLatin1CharacterSet()->GetScale();

                    if (pValues->kerning && (strit < (g_context.wideText.end() - 1)))
                    {
                        uint32_t next = *(strit + 1);

                        if (Functions::IsLatin1Char(next))
                        {
                            CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
                            float fKerning;

                            __asm
                            {
                                push next;
                                push unicode;
                                mov ecx, pSet;
                                call g_game.pfCBitmapCharacterSet_GetKerning;
                                movss fKerning, xmm0;
                            }

                            vTempWidth += fKerning;
                        }
                    }

                    if (pValues->h == 0 || !Functions::IsLatin1Char(unicode))
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
        //xmm0 fIconWidth 常量8.0
        //xmm1 临时变量
        //xmm2 vWordWidth 当前单词宽度
        //xmm3 vTempWidth 当前行宽度
        //xmm4 vCurrentLineHeight 当前行高度
        //xmm5 vHeight 结果
        //xmm6 vCharaterHeight 默认行高度

        static const float fIconWidth = 8.0f;

        char Tag[128];
        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;
        float vCharacterHeight = injector::thiscall<int(CBitmapFont *)>::vtbl<12>(pFont);
        float vHeight = vCharacterHeight;
        float vCurrentLineHeight = vCharacterHeight;
        g_context.pSet = pFont->GetLatin1CharacterSet();

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        nMaxWidth -= 2 * BorderSize->x;

        std::string_view text_view(text->c_str());

        if (text_view.empty())
        {
            return vHeight;
        }

        g_context.cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        g_context.wideText.clear();
        utf8::unchecked::utf8to32(text_view.begin(), text_view.end(), back_inserter(g_context.wideText));

        for (auto strit = g_context.wideText.begin(); strit < g_context.wideText.end(); ++strit)
        {
            auto unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x7))
            {
                if (unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < g_context.wideText.end()))
                    {
                        Tag[index] = *strit;
                        ++index;
                        ++strit;
                    }

                    Tag[index] = 0;

                    float fWidth = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, Tag);
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;

                    if (pFont->get_field<bool, 0x24F0>())
                    {
                        float fHeight = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<29>(pFont, Tag);
                        vCurrentLineHeight = max(vCurrentLineHeight, fHeight);
                    }
                }
                else if (unicode == 0x7)
                {
                    ++strit;
                }
                else if (unicode == 0x40)
                {
                    strit += 3;

                    float fWidth = injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;
                }
                else if (unicode == 0x7B)
                {
                    strit += 3;
                    vWordWidth += fIconWidth;
                    vTempWidth += fIconWidth;
                }
            }
            else
            {
                const EU4CharacterValues *pValues;

                if (unicode == ' ' || !Functions::IsLatin1Char(unicode))
                {
                    vWordWidth = 0.0f;
                }

                if (Functions::IsLatin1Char(unicode))
                {
                    pValues = pFont->GetLatin1Value(unicode);
                }
                else
                {
                    pValues = &g_context.cjkFont->GetValue(unicode)->EU4Values;
                }

                if (pValues == nullptr)
                {
                    if (unicode == 0xA || unicode == 0xD)
                    {
                        vHeight += vCurrentLineHeight;
                        vCurrentLineHeight = vCharacterHeight;
                        vWordWidth = 0.0f;
                        vTempWidth = 0.0f;
                    }
                }
                else
                {
                    float fWidth = pValues->xadvance * pFont->GetLatin1CharacterSet()->GetScale();
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;

                    if (pValues->kerning)
                    {
                        if (strit < (g_context.wideText.end() - 1))
                        {
                            auto next = strit[1];

                            if (Functions::IsLatin1Char(next))
                            {
                                float fKerning;

                                __asm
                                {
                                    push next;
                                    push unicode;
                                    mov ecx, g_context.pSet;
                                    call g_game.pfCBitmapCharacterSet_GetKerning;
                                    movss fKerning, xmm0;
                                }

                                vWordWidth += fKerning;
                                vTempWidth += fKerning;
                            }
                        }
                    }

                    if (vTempWidth >= nMaxWidth)
                    {
                        vHeight += vCurrentLineHeight;
                        vCurrentLineHeight = vCharacterHeight;
                        vTempWidth = vWordWidth;
                        vWordWidth = 0.0f;
                    }
                }
            }
        }

        return vHeight;
    }

    static int __fastcall GetActualRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, int nMaxWidth, CVector2<unsigned int> *BorderSize, CVector2<short> *NeededSize)
    {
        static const float fIconWidth = 8.0f;

        g_context.pSet = pFont->GetLatin1CharacterSet();
        g_context.cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        char Tag[128];

        int nLinesHeight[110] = { 0 };
        int nLines = 0;
        int nWidth = 0;
        int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * g_context.pSet->GetScale();
        int nCurrentLineHeight = nDefaultLineHeight;
        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        if (OriginalText->length() != 0)
        {
            std::string_view source_view{ OriginalText->c_str() };

            g_context.wideText.clear();
            utf8::utf8to32(source_view.begin(), source_view.end(), std::back_inserter(g_context.wideText));

            for (auto strit = g_context.wideText.begin(); (strit < g_context.wideText.end()) && nLines != 52; ++strit)
            {
                g_context.unicode = *strit;

                if (g_context.unicode == 0x7)
                {
                    ++strit;
                }
                else if (g_context.unicode == 0x7B)
                {
                    strit += 3;
                    vWordWidth += fIconWidth;
                    vTempWidth += fIconWidth;
                }
                else if (g_context.unicode == 0x40)
                {
                    strit += 3;

                    float fWidth = injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;
                }
                else if (g_context.unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < g_context.wideText.end()))
                    {
                        Tag[index] = *strit;
                        ++index;
                        ++strit;
                    }

                    Tag[index] = 0;

                    float fWidth = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, Tag);
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;

                    if (pFont->get_field<bool, 0x24F0>())
                    {
                        float fHeight = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<29>(pFont, Tag);
                        nCurrentLineHeight = max(nCurrentLineHeight, fHeight);
                    }
                }
                else
                {
                    const EU4CharacterValues *pValues;

                    if (Functions::IsLatin1Char(g_context.unicode))
                    {
                        pValues = g_context.pSet->GetLatin1Value(g_context.unicode);
                    }
                    else
                    {
                        pValues = &g_context.cjkFont->GetValue(g_context.unicode)->EU4Values;
                    }

                    if (pValues)
                    {
                        float fWidth = pValues->xadvance * g_context.pSet->GetScale();
                        vWordWidth += fWidth;
                        vTempWidth += fWidth;

                        if (pValues->kerning && strit < (g_context.wideText.end() - 1))
                        {
                            uint32_t next = strit[1];

                            if (Functions::IsLatin1Char(next))
                            {
                                float fKerning;

                                __asm
                                {
                                    push next;
                                    push g_context.unicode;
                                    mov ecx, g_context.pSet;
                                    call g_game.pfCBitmapCharacterSet_GetKerning;
                                    movss fKerning, xmm0;
                                }

                                vWordWidth += fKerning;
                                vTempWidth += fKerning;
                            }
                        }

                        if (pValues->h == 0 || Functions::IsLatin1Char(g_context.unicode))
                        {
                            if (vTempWidth > nMaxWidth)
                            {
                                nWidth = max(nWidth, vTempWidth);

                                nLinesHeight[nLines] = nCurrentLineHeight;
                                ++nLines;
                                nCurrentLineHeight = nDefaultLineHeight;
                                vTempWidth = vWordWidth;
                            }

                            vWordWidth = 0.0f;
                            nWidth = max(vTempWidth, nWidth);
                        }
                        else
                        {
                            int width = (nMaxWidth - 2 * BorderSize->x);

                            if (vTempWidth >= width  && vWordWidth > 50.0f)
                            {
                                nLinesHeight[nLines] = nCurrentLineHeight;
                                ++nLines;
                                nCurrentLineHeight = nDefaultLineHeight;
                                vTempWidth = vWordWidth;
                                vWordWidth = 0.0f;
                                nWidth = width;
                            }
                        }
                    }
                    else
                    {
                        if (g_context.unicode == 0xA || g_context.unicode == 0xD)
                        {
                            nWidth = max(nWidth, vTempWidth);

                            nLinesHeight[nLines] = nCurrentLineHeight;
                            ++nLines;
                            nCurrentLineHeight = nDefaultLineHeight;
                            vWordWidth = 0.0f;
                            vTempWidth = 0.0f;
                        }
                    }
                }
            }
        }

        nWidth = max(vTempWidth, nWidth);

        if (nWidth > nMaxWidth)
        {
            nWidth = nMaxWidth;
            nLinesHeight[nLines] = nCurrentLineHeight;
            ++nLines;
            nLinesHeight[nLines] = nDefaultLineHeight;
        }
        else
        {
            nLinesHeight[nLines] = nCurrentLineHeight;
        }

        ++nLines;

        int nSumHeight = std::accumulate(std::begin(nLinesHeight), std::end(nLinesHeight), 0);

        NeededSize->x = (nWidth + BorderSize->x * 2);
        NeededSize->y = BorderSize->y * 2 + (BorderSize->y < 0 ? 30.0f : 0.0f) + nSumHeight;

        return nLines;
    }

    static void __fastcall GetRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<unsigned int> *BorderSize, bool bUseSpecialChars)
    {
        char Tag[128];

        std::string Result{ OriginalText->c_str() };
        g_context.narrowText.clear();

        g_context.pSet = pFont->GetLatin1CharacterSet();
        g_context.cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        nMaxWidth -= 2 * BorderSize->x;
        nMaxHeight -= 2 * BorderSize->x;



        NewText->assign(g_context.narrowText.c_str());
    }

    static void __fastcall GetActualRealRequiredSizeActually(CBitmapFont *pFont, int, const CString *OriginalText, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<unsigned int> *BorderSize, bool bWholeWordOnly, bool bAddBreaksToNewText, bool bUseSpecialChars)
    {
        std::string Result{ OriginalText->c_str() };
        g_context.narrowText.clear();



        NewText->assign(g_context.narrowText.c_str());
    }

    void InitAndPatch()
    {
#pragma region 整个换掉的函数
        injector::MakeJMP(g_pattern.find_pattern("81 EC 8C 00 00 00 53 8B 5D 0C").get(0).integer(-6), GetWidthOfString);
        injector::MakeJMP(g_pattern.find_pattern("81 EC AC 00 00 00 8B 55 0C").get(0).integer(-6), GetHeightOfString);
        injector::MakeJMP(g_pattern.find_first("81 EC 0C 03 00 00 8B 45 0C").integer(-0x18), GetActualRequiredSize);
#pragma endregion 整个换掉的函数

#pragma region CBitmapFont_RenderToTexture

#pragma endregion CBitmapFont_RenderToTexture

#pragma region CBitmapFont_RenderToScreen
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
#pragma endregion CBitmapFont_RenderToScreen

#pragma region 0xA3 0xA4 0xA7
        //cmp byte ptr [eax + esi], 0xA?
        g_pattern.find_pattern("80 3C 30 A?").for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(3);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //cmp byte ptr [eax + edi], 0xA?
        g_pattern.find_pattern("80 3C 38 A?").for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(3);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //cmp al, 0xA?; jnz short
        g_pattern.find_pattern("3C A? 75").for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(1);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //cmp al, 0xA?; jnz
        g_pattern.find_pattern("3C A? 0F 85").for_each_result( 
            [](memory_pointer p)
        {
            uint8_t *pb = p.raw<uint8_t>(1);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //-----------------------------------------------------漏网之鱼-----------------------------------------------------
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("3C A7 8D 85 84").get(0).integer(1), 7, true); //cmp al, 0A7h; lea eax, [ebp - 0x7C]
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("80 3C 31 A7").get(0).integer(3), 7, true); //cmp byte ptr [ecx + esi], 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("80 7C 06 FF A7").get(0).integer(4), 7, true); //cmp byte ptr[esi + eax -1], 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("80 3C 06 A7").get(0).integer(3), 7, true); //cmp byte ptr[esi + eax], 0xA7

        g_pattern.find_pattern("A3 61 64 6D").for_each_result( //£adm
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.raw<uint8_t>(), 3, true);
        });

        g_pattern.find_pattern("A3 64 69 70").for_each_result( //£dip
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.raw<uint8_t>(), 3, true);
        });

        g_pattern.find_pattern("A3 6D 69 6C").for_each_result( //£mil
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.raw<uint8_t>(), 3, true);
        });

#pragma endregion 0xA3 0xA4 0xA7

#pragma region RemoveSpecialChars
        //待定
        injector::WriteMemory(g_pattern.find_pattern("83 EC 34 6A 05 68").get(0).integer(6), "@{", true);
#pragma endregion RemoveSpecialChars
    }
}
