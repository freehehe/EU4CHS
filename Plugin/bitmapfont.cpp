#include "eu4.h"
#include "functions.h"
#include "cjk_fonts.h"
#include "cjk_normal.h"
#include "bitmapfont.h"
#include "context.h"

namespace BitmapFont
{
    //CBitmapFont_RenderToScreen
    //ReadWord
    //8A 87 ? ? ? ? 88 86 ? ? ? ? 46
    //13
    //0x1098CA0
    struct CBitmapFont_RenderToScreen_ReadWord
    {
        void operator()(injector::reg_pack *regs) const
        {
            g_context.pFont = *(CBitmapFont **)(regs->ebp.i - 0x10);
            g_context.pSet = g_context.pFont->GetLatin1CharacterSet();
            g_context.cjkFont = g_Fonts.GetFont(g_context.pFont->GetFontPath());

            g_context.textLength = *(uint32_t *)(regs->ebp.i - 0x14);
            g_context.useSpecialChars = *(bool *)(regs->ebp.i + 0x3C);

            Functions::GetTwoUnicode(
                g_game.pOriginalText,
                regs->edi.i,
                g_context.textLength,
                g_context.unicode,
                g_context.unicodeLength,
                g_context.nextUnicode,
                g_context.useSpecialChars);

            regs->eax.i = g_context.unicode;
            utf8::append(g_context.unicode, g_game.pWord + regs->esi.i);
            regs->esi.i += g_context.unicodeLength;
            regs->edi.i += (g_context.unicodeLength - 1);
            *(uint32_t *)(regs->ebp.i - 0x30) = regs->edi.i;

            if (!Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ecx = 0;
            }
        }
    };

    //CBitmapFont_RenderToScreen
    //GetCharInfo1
    //0F B6 C0 8B 0C 82
    //9
    //0x1098E6E
    struct CBitmapFont_RenderToScreen_GetCharInfo1
    {
        void operator()(injector::reg_pack *regs) const
        {
            regs->eax = g_context.unicode;

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ecx = g_context.pSet->GetLatin1Value(g_context.unicode);
            }
            else
            {
                regs->ecx = g_context.cjkFont->GetEU4Value(g_context.unicode);
            }
        }
    };

    //CBitmapFont_RenderToScreen
    //Delim
    //66 83 79 06 00
    //5
    //0x1098F0A
    struct CBitmapFont_RenderToScreen_Delim
    {
        void operator()(injector::reg_pack *regs) const
        {
            EU4CharInfo *pInfo = regs->ecx;

            if (pInfo->h == 0 || !Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ef.zero_flag = true;
            }
            else
            {
                regs->ef.zero_flag = false;
            }
        }
    };

    //CBitmapFont_RenderToScreen
    //GetCharInfo2
    //8A 8F ? ? ? ? 8B 55 CC
    //12 3
    //0x1099DA7 0x1099DBB
    struct CBitmapFont_GetCharInfo2
    {
        void operator()(injector::reg_pack *regs) const
        {
            g_context.textLength = *(uint32_t *)(regs->ebp.i - 0x30);
            g_context.useSpecialChars = *(bool *)(regs->ebp.i + 0x3C);

            Functions::GetTwoUnicode(
                g_game.pText,
                regs->edi.i,
                g_context.textLength,
                g_context.unicode,
                g_context.unicodeLength,
                g_context.nextUnicode,
                g_context.useSpecialChars);

            regs->edx = g_context.pSet;
            regs->ecx = g_context.unicode;
            regs->eax = g_context.unicode;

            regs->edi.i += (g_context.unicodeLength - 1);

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->esi = g_context.pSet->GetLatin1Value(g_context.unicode);
            }
            else
            {
                regs->esi = g_context.cjkFont->GetEU4Value(g_context.unicode);
            }
        }
    };

    //CBitmapFont_RenderToScreen
    //GenVertices
    //B8 AB AA AA 2A F7 E9 8B C2 C1 E8 1F 03 C2 3D C2 00 00 00
    //14
    //0x109A654
    struct CBitmapFont_RenderToScreen_GenVertices
    {
        void operator()(injector::reg_pack *regs) const
        {
            int *nVertexCount = (int *)(regs->ebp.i - 0x40);

            if (!Functions::IsLatin1Char(g_context.unicode))
            {
                *nVertexCount -= 6;

                STextVertex *pVertices = g_game.pBitmapVertices + *nVertexCount;

                g_context.cjkFont->AddVerticesDX9(g_context.pFont, g_context.unicode, pVertices);
            }

            regs->eax.i = *nVertexCount / 6;
        }
    };

    //CBitmapFont_RenderToScreen
    //RenderFull
    //6A 00 6A 00 FF 75 D8 FF 15 ? ? ? ? F3 0F 10 55 08
    //6
    //0x109A6AF
    struct CBitmapFont_RenderToScreen_RenderFull
    {
        void operator()(injector::reg_pack *regs) const
        {
            injector::cstd<void(uint32_t, int, int)>::call(g_game.pfGfxDrawDX9, *(uint32_t *)(regs->ebp.i - 0x28), 0, 0);
            g_context.cjkFont->DrawUnbufferedDX9();
        }
    };

    //CBitmapFont_RenderToScreen
    //RenderRest
    //6A 00 57 56 FF 15 ? ? ? ? 83 C4 44
    //6
    //0x109A758
    struct CBitmapFont_RenderToScreen_RenderRest
    {
        void operator()(injector::reg_pack *regs) const
        {
            injector::cstd<void(uint32_t, int, int)>::call(g_game.pfGfxDrawDX9, regs->esi.i, regs->edi.i, 0);
            g_context.cjkFont->DrawUnbufferedDX9();
        }
    };

    //CBitmapFont_RenderToTexture
    //GetCharInfo1
    //8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00
    //13
    //0x1096FF9
    struct CBitmapFont_RenderToTexture_GetCharInfo1
    {
        void operator()(injector::reg_pack *regs) const
        {
            g_context.pFont = regs->edi;
            g_context.pSet = g_context.pFont->GetLatin1CharacterSet();
            g_context.cjkFont = g_Fonts.GetFont(g_context.pFont->GetFontPath());

            g_context.textLength = *(uint32_t *)(regs->ebp.i - 0x28);
            g_context.useSpecialChars = *(bool *)(regs->ebp.i + 0x3C);

            Functions::GetTwoUnicode(
                regs->eax,
                regs->esi.i,
                g_context.textLength,
                g_context.unicode,
                g_context.unicodeLength,
                g_context.nextUnicode,
                g_context.useSpecialChars);

            regs->esi.i += (g_context.unicodeLength - 1);
            
            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->eax = g_context.pSet->GetLatin1Value(g_context.unicode);
            }
            else
            {
                regs->eax = g_context.cjkFont->GetEU4Value(g_context.unicode);
            }
        }
    };

    //CBitmapFont_RenderToTexture
    //Delim
    //66 83 78 06 00
    //5
    //0x10970A4
    struct CBitmapFont_RenderToTexture_Delim
    {
        void operator()(injector::reg_pack *regs) const
        {
            EU4CharInfo *pInfo = regs->eax;

            if (pInfo->h == 0 || !Functions::IsLatin1Char(g_context.unicode))
            {
                regs->ef.zero_flag = true;
            }
            else
            {
                regs->ef.zero_flag = false;
            }
        }
    };

    //CBitmapFont_RenderToTexture
    //GetCharInfo2
    //0F B6 04 38 8B 04 81
    //7
    //0x1097FD4
    struct CBitmapFont_RenderToTexture_GetCharInfo2
    {
        void operator()(injector::reg_pack *regs) const
        {
            g_context.textLength = *(uint32_t *)(regs->ebp.i - 0x38);
            g_context.useSpecialChars = *(bool *)(regs->ebp.i + 0x3C);

            Functions::GetTwoUnicode(
                regs->eax,
                regs->edi.i,
                g_context.textLength,
                g_context.unicode,
                g_context.unicodeLength,
                g_context.nextUnicode,
                g_context.useSpecialChars);

            regs->edi.i += (g_context.unicodeLength - 1);
            *(uint32_t *)(regs->ebp.i - 0x10) = regs->edi.i;

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->eax = g_context.pSet->GetLatin1Value(g_context.unicode);
            }
            else
            {
                regs->eax = g_context.cjkFont->GetEU4Value(g_context.unicode);
            }
        }
    };

    //CBitmapFont_RenderToTexture
    //GenVertices
    //8B 55 24 8B 7D F0 42
    //7
    //0x10983CE
    struct CBitmapFont_RenderToTexture_GenVertices
    {
        void operator()(injector::reg_pack *regs) const
        {
            regs->edx = *(uint32_t *)(regs->ebp.i + 0x24);
            regs->edi = *(uint32_t *)(regs->ebp.i - 0x10);

            STextVertex *pVertices = *(STextVertex **)(regs->ebp.i + 0x14) + regs->edx.i * 6;

            if (Functions::IsLatin1Char(g_context.unicode))
            {
                regs->edx.i += 1;
            }
            else
            {
                g_context.cjkFont->AddVerticesDX9(g_context.pFont, g_context.unicode, pVertices);
            }
        }
    };

    //CBitmapFont_RenderToTexture
    //Render
    //6A 00 56 FF 75 10 FF 15
    //6
    //0x1098506
    struct CBitmapFont_RenderToTexture_Render
    {
        void operator()(injector::reg_pack *regs) const
        {
            injector::cstd<void(uint32_t, int, int)>::call(g_game.pfGfxDrawDX9, *(uint32_t *)(regs->ebp.i + 0x10), regs->esi.i, 0);
            g_context.cjkFont->DrawUnbufferedDX9();
        }
    };

    //NAME
    //USAGE
    //PATTERN
    //NOPLENGTH
    //ADDRESS
    struct NAME_USAGE
    {
        void operator()(injector::reg_pack *regs) const
        {

        }
    };

    static int __fastcall GetWidthOfString(CBitmapFont * pFont, int edx, const char * Text, const int nLength, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        char Tag[128];
        float vTempWidth = 0.0f; //当前行宽度
        int nWidth = 0; //最大行宽度

        CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
        CJKFont *cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        int nRealSize = nLength;

        if (nLength < 0)
        {
            nRealSize = strlen(Text);
        }

        std::vector<wchar_t> wideText;
        utf8::utf8to16(Text, Text + nRealSize, back_inserter(wideText));

        for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
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

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < wideText.end()))
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
                const EU4CharInfo *pValues;

                if (Functions::IsLatin1Char(unicode))
                {
                    pValues = pFont->GetLatin1Value(unicode);
                }
                else
                {
                    pValues = cjkFont->GetEU4Value(unicode);
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
                    vTempWidth += pValues->xadvance * pSet->GetScale();

                    if (pValues->h == 0 || !Functions::IsLatin1Char(unicode))
                    {
                        nWidth = max(nWidth, vTempWidth);
                    }
                }
            }
        }

        return max(nWidth, vTempWidth);
    }

    static int __fastcall GetHeightOfString(CBitmapFont * pFont, int edx, const CString *text, int nMaxWidth, int nMaxHeight, const CVector2<int> *BorderSize, bool bUseSpecialChars)
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
        CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
        CJKFont *cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

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

        std::vector<wchar_t> wideText;
        utf8::utf8to16(text_view.begin(), text_view.end(), back_inserter(wideText));

        for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
        {
            auto unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x7))
            {
                if (unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < wideText.end()))
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
                const EU4CharInfo *pValues;

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
                    pValues = cjkFont->GetEU4Value(unicode);
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

    static int __fastcall GetActualRequiredSize(CBitmapFont *pFont, int edx, const CString *OriginalText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, CVector2<short> *NeededSize, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
        CJKFont *cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        char Tag[128];
        int nLinesHeight[110] = { 0 };
        int nLines = 0;
        int nWidth = 0;
        int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScale();
        int nCurrentLineHeight = nDefaultLineHeight;
        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }
        
        if (OriginalText->length() > 0)
        {
            std::string_view source_view{ OriginalText->c_str() };

            std::vector<wchar_t> wideText;
            utf8::utf8to16(source_view.begin(), source_view.end(), std::back_inserter(wideText));

            for (auto strit = wideText.begin(); strit < wideText.end() && nLines != 52; ++strit)
            {
                auto unicode = *strit;

                if (unicode == 0x7)
                {
                    ++strit;
                }
                else if (unicode == 0x7B)
                {
                    strit += 3;
                    vWordWidth += fIconWidth;
                    vTempWidth += fIconWidth;
                }
                else if (unicode == 0x40)
                {
                    strit += 3;

                    float fWidth = injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;
                }
                else if (unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < wideText.end()))
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
                        int Height = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<29>(pFont, Tag);
                        nCurrentLineHeight = max(nCurrentLineHeight, Height);
                    }
                }
                else
                {
                    const EU4CharInfo *pValues;

                    if (Functions::IsLatin1Char(unicode))
                    {
                        pValues = pSet->GetLatin1Value(unicode);
                    }
                    else
                    {
                        pValues = cjkFont->GetEU4Value(unicode);
                    }

                    if (pValues)
                    {
                        float fWidth = pValues->xadvance * pSet->GetScale();
                        vWordWidth += fWidth;
                        vTempWidth += fWidth;

                        if (pValues->h == 0 || !Functions::IsLatin1Char(unicode))
                        {
                            if (vTempWidth > nMaxWidth)
                            {
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

                            if (vTempWidth >= width && vWordWidth > 50.0f)
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
                        if (unicode == 0xA || unicode == 0xD)
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
        NeededSize->y = (BorderSize->y * 2 + (BorderSize->y < 0 ? 30.0f : 0.0f) + nSumHeight);

        return nLines;
    }

    static void __fastcall GetRequiredSize(CBitmapFont *pFont, int edx, const CString *OriginalText, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        //edi OriginalText
        //ebx: NewText
        //xmm0 8.0
        //xmm2 vTempWidth
        //xmm3 vWordWidth
        //xmm4 vHeight

        char Tag[128];

        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;

        CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
        CJKFont *cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        std::string Result{ OriginalText->c_str() };

        std::vector<wchar_t> wideText;
        utf8::utf8to16(Result.begin(), Result.end(), std::back_inserter(wideText));

        int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScale();
        int nCurrentLineHeight = nDefaultLineHeight;
        float vHeight = nDefaultLineHeight;
        int nWidth = 0;

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        nMaxWidth -= 2 * BorderSize->x;
        nMaxHeight -= 2 * BorderSize->x;

        if (OriginalText->length() == 0)
        {
            NewText->assign(Result.c_str());
            return;
        }

        for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
        {
            auto unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x7))
            {
                if (unicode == 0x40)
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
                else if (unicode == 0x3)
                {
                    ++strit;

                    size_t index = 0;

                    while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < wideText.end()))
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
                        int Height = injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<29>(pFont, Tag);
                        nCurrentLineHeight = max(nCurrentLineHeight, Height);
                    }
                }
                else if (unicode == 0x7)
                {
                    ++strit;
                }
            }
            else
            {
                const EU4CharInfo *pValues;

                if (Functions::IsLatin1Char(unicode))
                {
                    pValues = pFont->GetLatin1Value(unicode);
                }
                else
                {
                    pValues = cjkFont->GetEU4Value(unicode);
                }

                if (pValues)
                {
                    float fWidth = pValues->xadvance * pSet->GetScale();
                    vWordWidth += fWidth;
                    vTempWidth += fWidth;

                    if (vTempWidth >= nMaxWidth)
                    {
                        vHeight += nCurrentLineHeight;
                        nCurrentLineHeight = nDefaultLineHeight;
                        vTempWidth = vWordWidth;

                        if (vHeight > nMaxHeight)
                        {
                            if (strit - wideText.begin() > 3)
                            {
                                Result.clear();
                                utf8::utf16to8(wideText.begin(), strit - 4, back_inserter(Result));
                                Result += " ...";
                            }
                            else
                            {
                                Result = "...";
                                return;
                            }

                            NewText->assign(Result.c_str());
                            return;
                        }
                        else
                        {
                            vWordWidth = 0.0f;
                            nWidth = max(vTempWidth, nWidth);
                        }
                    }
                }
                else
                {
                    if (unicode == 0xA || unicode == 0xD)
                    {
                        nWidth = max(vTempWidth, nWidth);
                        vHeight += nCurrentLineHeight;
                        vWordWidth = 0.0f;
                        nCurrentLineHeight = nDefaultLineHeight;
                        vTempWidth = 0.0f;

                        if (vHeight > nMaxHeight)
                        {
                            if ((strit - wideText.begin()) > 4)
                            {
                                Result.clear();
                                utf8::utf16to8(wideText.begin(), strit - 4, back_inserter(Result));
                                Result += " ...";
                            }
                            else
                            {
                                Result.clear();
                                utf8::utf16to8(wideText.begin(), strit, back_inserter(Result));
                            }

                            NewText->assign(Result.c_str());
                            return;
                        }
                    }
                }
            }
        }

        NewText->assign(Result.c_str());
    }

    static void __fastcall GetActualRealRequiredSizeActually(CBitmapFont *pFont, int edx, const CString *Text, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bWholeWordOnly, bool bAddBreaksToNewText, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        //edi Text
        //ebx NewText
        //xmm2 vWordWidth
        //xmm3 vTempWidth
        //xmm4 vHeight

        char Tag[128];

        CBitmapCharacterSet *pSet = pFont->GetLatin1CharacterSet();
        CJKFont *cjkFont = g_Fonts.GetFont(pFont->GetFontPath());

        int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScale();
        int nCurrentLineHeight = nDefaultLineHeight;
        int nWidth = 0;

        float vWordWidth = 0.0f;
        float vTempWidth = 0.0f;
        float vHeight = nDefaultLineHeight;

        std::string Result;

        if (nMaxWidth == 0)
        {
            nMaxWidth = 320;
        }

        nMaxWidth -= 2 * BorderSize->x;
        nMaxHeight -= 2 * BorderSize->y;

        std::vector<wchar_t> wideText;
        std::string_view source_view{ Text->c_str() };
        utf8::utf8to16(source_view.begin(), source_view.end(), std::back_inserter(wideText));

        std::vector<wchar_t>::iterator nLastIndexOfWholeWord = wideText.begin();
        std::vector<wchar_t>::iterator nCurrentLineStartIndex = wideText.begin();

        if (Text->length() != 0)
        {
            for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
            {
                auto unicode = *strit;

                if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0x3 || unicode == 0x7))
                {
                    if (unicode == 0x40)
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
                    else if (unicode == 0x3)
                    {
                        ++strit;

                        size_t index = 0;

                        while (Functions::IsTextIconChar(*strit) && (index < 127) && (strit < wideText.end()))
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
                    else if (unicode == 0x7)
                    {
                        ++strit;
                    }
                }
                else
                {
                    if (unicode == ' ' || !Functions::IsLatin1Char(unicode))
                    {
                        vWordWidth = 0.0f;
                        nLastIndexOfWholeWord = strit;
                    }

                    const EU4CharInfo *pValues;

                    if (Functions::IsLatin1Char(unicode))
                    {
                        pValues = pFont->GetLatin1Value(unicode);
                    }
                    else
                    {
                        pValues = cjkFont->GetEU4Value(unicode);
                    }

                    if (pValues)
                    {
                        float fWidth = pValues->xadvance * pSet->GetScale();
                        vWordWidth += fWidth;
                        vTempWidth += fWidth;

                        if (vTempWidth >= nMaxWidth)
                        {
                            vHeight += nCurrentLineHeight;
                            nCurrentLineHeight = nDefaultLineHeight;
                            vTempWidth = vWordWidth;

                            if (bAddBreaksToNewText)
                            {
                                if (nLastIndexOfWholeWord > nCurrentLineStartIndex)
                                {
                                    utf8::utf16to8(nCurrentLineStartIndex, nLastIndexOfWholeWord, back_inserter(Result));
                                    Result += '\n';
                                    nCurrentLineStartIndex = nLastIndexOfWholeWord + 1;
                                }
                                else
                                {
                                    utf8::utf16to8(nCurrentLineStartIndex, strit, back_inserter(Result));
                                    Result += '\n';
                                    nLastIndexOfWholeWord = strit;
                                    nCurrentLineStartIndex = strit;
                                    vTempWidth = 0.0f;
                                }
                            }

                            if (vHeight > nMaxHeight)
                            {
                                if (!bAddBreaksToNewText)
                                {
                                    if (bWholeWordOnly)
                                    {
                                        utf8::utf16to8(wideText.begin(), nLastIndexOfWholeWord, back_inserter(Result));
                                    }
                                    else
                                    {
                                        if (strit - wideText.begin() > 4)
                                        {
                                            utf8::utf16to8(wideText.begin(), strit, back_inserter(Result));
                                        }
                                        else
                                        {
                                            utf8::utf16to8(wideText.begin(), strit - 4, back_inserter(Result));
                                            Result += " ...";
                                        }
                                    }
                                }

                                NewText->assign(Result.c_str());
                                return;
                            }
                            else
                            {
                                vWordWidth = 0.0f;
                            }
                        }
                    }
                    else
                    {
                        if (unicode == 0xA || unicode == 0xD)
                        {
                            vHeight += nCurrentLineHeight;
                            nCurrentLineHeight = nDefaultLineHeight;
                            vWordWidth = 0.0f;
                            vTempWidth = 0.0f;
                            nLastIndexOfWholeWord = strit;

                            if (bAddBreaksToNewText)
                            {
                                utf8::utf16to8(nCurrentLineStartIndex, strit, back_inserter(Result));
                                Result += '\n';

                                nCurrentLineStartIndex = strit + 1;
                            }

                            if (vHeight > nMaxHeight)
                            {
                                if (!bAddBreaksToNewText)
                                {
                                    if (!bWholeWordOnly && (strit - wideText.begin()) > 4)
                                    {
                                        utf8::utf16to8(wideText.begin(), strit - 4, std::back_inserter(Result));
                                        Result += " ...";
                                    }
                                    else
                                    {
                                        utf8::utf16to8(wideText.begin(), strit, std::back_inserter(Result));
                                    }
                                }

                                NewText->assign(Result.c_str());
                                return;
                            }
                        }
                    }
                }
            }
        }

        if (bAddBreaksToNewText)
        {
            utf8::utf16to8(nCurrentLineStartIndex, wideText.end(), std::back_inserter(Result));
            NewText->assign(Result.c_str());
        }
        else
        {
            NewText->assign(Text->c_str());
        }
    }

    void InitAndPatch()
    {
        //去掉Kerning
        injector::MakeNOP(g_pattern.find_first("C6 40 0E 01 5F").integer(), 4);

        //整个换掉的函数
        injector::MakeJMP(g_pattern.find_first("81 EC 8C 00 00 00 53 8B 5D 0C").integer(-6), GetWidthOfString);
        injector::MakeJMP(g_pattern.find_first("81 EC AC 00 00 00 8B 55 0C").integer(-6), GetHeightOfString);
        injector::MakeJMP(g_pattern.find_first("81 EC 0C 03 00 00 8B 45 0C").integer(-0x18), GetActualRequiredSize);
        injector::MakeJMP(g_pattern.find_first("81 EC CC 00 00 00 53 56 57 8B 7D 08 89 4D F0").integer(-0x18), GetRequiredSize);
        injector::MakeJMP(g_pattern.find_first("81 EC 04 01 00 00 53 8B 5D 0C 56").integer(-0x18), GetActualRealRequiredSizeActually);

        //RenderToScreen
        //0x1098610

        //取消缓存
        injector::MakeNOP(g_pattern.find_first("0F 84 B8 1B 00 00").pointer(), 6);
        injector::WriteMemory<char>(g_pattern.find_first("C6 45 3B 01").pointer(3), 0, true);

        g_pattern.find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
        injector::MakeInline<CBitmapFont_RenderToScreen_ReadWord>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(13));

        g_pattern.find_pattern("0F B6 C0 8B 0C 82");
        injector::MakeInline<CBitmapFont_RenderToScreen_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(9));

        g_pattern.find_pattern("66 83 79 06 00");
        injector::MakeInline<CBitmapFont_RenderToScreen_Delim>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(5));

        g_pattern.find_pattern("8A 8F ? ? ? ? 8B 55 CC");
        injector::MakeNOP(g_pattern.get_first().pointer(20), 3);
        injector::MakeInline<CBitmapFont_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(12));

        g_pattern.find_pattern("B8 AB AA AA 2A F7 E9 8B C2 C1 E8 1F 03 C2 3D C2 00 00 00");
        injector::MakeInline<CBitmapFont_RenderToScreen_GenVertices>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(14));

        g_pattern.find_pattern("6A 00 6A 00 FF 75 D8 FF 15 ? ? ? ? F3 0F 10 55 08");
        injector::MakeInline<CBitmapFont_RenderToScreen_RenderFull>(g_pattern.get_first().integer(7), g_pattern.get_first().integer(13));

        g_pattern.find_pattern("6A 00 57 56 FF 15 ? ? ? ? 83 C4 44");
        injector::MakeInline<CBitmapFont_RenderToScreen_RenderRest>(g_pattern.get_first().integer(4), g_pattern.get_first().integer(10));

        //RenderToTexture
        //0x1096A60
        g_pattern.find_pattern("8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00");
        injector::MakeInline<CBitmapFont_RenderToTexture_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(13));

        g_pattern.find_pattern("66 83 78 06 00");
        injector::MakeInline<CBitmapFont_RenderToTexture_Delim>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(5));

        g_pattern.find_pattern("0F B6 04 38 8B 04 81");
        injector::MakeInline<CBitmapFont_RenderToTexture_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));

        g_pattern.find_pattern("8B 55 24 8B 7D F0 42");
        injector::MakeInline<CBitmapFont_RenderToTexture_GenVertices>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));

        g_pattern.find_pattern("6A 00 56 FF 75 10 FF 15");
        injector::MakeInline<CBitmapFont_RenderToTexture_Render>(g_pattern.get_first().pointer(6), g_pattern.get_first().pointer(12));

        //FillVertexBuffer
        //0x109CB80

        //g_pattern.find_pattern("");
        //injector::MakeInline<>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer());

        //A3 A4 A7
        //cmp byte ptr [eax + esi], 0xA?
        g_pattern.find_pattern("80 3C 30 A?").for_each_result(
            [](memory_pointer p)
        {
            uint8_t *pb = p.pointer<uint8_t>(3);
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
            uint8_t *pb = p.pointer<uint8_t>(3);
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
            uint8_t *pb = p.pointer<uint8_t>(1);
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
            uint8_t *pb = p.pointer<uint8_t>(1);
            uint8_t b = *pb;

            if (b == 0xA3 || b == 0xA4 || b == 0xA7)
            {
                injector::WriteMemory<uint8_t>(pb, b - 0xA0, true);
            }
        });

        //cmp al, 0A7h; lea eax, [ebp - 0x7C]
        injector::WriteMemory<uint8_t>(g_pattern.find_first("3C A7 8D 45 84").integer(1), 7, true); 

        //cmp byte ptr[esi + eax -1], 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_first("80 7C 06 FF A7").integer(4), 7, true);

        //cmp byte ptr[esi + eax], 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_first("80 3C 06 A7").integer(3), 7, true);

        //WriteVariable 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_first("C6 06 A7").integer(2), 7, true);
        injector::WriteMemory<uint8_t>(g_pattern.find_first("66 C7 06 A7 21").integer(3), 7, true);

        //运行时连接字符串中的0xA3
        g_pattern.find_pattern("A3 61 64 6D").for_each_result( //£adm
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.integer(), 3, true);
        });

        g_pattern.find_pattern("A3 64 69 70").for_each_result( //£dip
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.integer(), 3, true);
        });

        g_pattern.find_pattern("A3 6D 69 6C").for_each_result( //£mil
            [](memory_pointer p)
        {
            injector::WriteMemory<uint8_t>(p.integer(), 3, true);
        });

        //CBitmapFont__RemoveSpecialChars
        injector::WriteMemory(g_pattern.find_first("83 EC 34 6A 05 68").integer(6), "@{", true);
    }
}
