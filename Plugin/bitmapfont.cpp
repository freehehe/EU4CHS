#include "eu4.h"
#include "functions.h"
#include "bitmapfont.h"
#include "std_string.h"
#include "context.h"

#define CHARSET_OFF 0xB4

EU4CharInfo * CBitmapCharacterSet::GetCharacterValue(uint32_t cp)
{
    auto *pCont = GetContainer();

    auto it = pCont->find(cp);

    if (it != pCont->end())
    {
        return &it->second;
    }
    else
    {
        return &pCont->find('?')->second;
    }
}

InfoContainer * CBitmapCharacterSet::GetContainer()
{
    return get_field<InfoContainer *, 0>();
}

float CBitmapCharacterSet::GetScaleX()
{
    return get_field<float, 0x428>();
}

CBitmapCharacterSet *CBitmapFont::GetCharacterSet()
{
    return field<CBitmapCharacterSet, CHARSET_OFF>();
}

InfoContainer *CBitmapFont::GetContainer()
{
    return GetCharacterSet()->GetContainer();
}

EU4CharInfo *CBitmapFont::GetCharacterValue(uint32_t cp)
{
    return GetCharacterSet()->GetCharacterValue(cp);
}

//CBitmapFont_ParseFontFile
//AllocValue
//6A 10 E8 ? ? ? ? 8B 4D D0 83 C4 04
//13
//89 84 8A B4 00 00 00
//7
//10968ED
struct CBitmapFont_ParseFontFile_AllocValue
{
    void operator()(injector::reg_pack &regs) const
    {
        CBitmapFont *pFont = *(CBitmapFont **)(regs.ebp.i - 0x10);
        uint32_t id = *(uint32_t *)(regs.ebp.i - 0x14);

        if (pFont->GetContainer() == nullptr)
        {
            *pFont->GetCharacterSet()->field<InfoContainer *, 0>() = new InfoContainer;
        }

        regs.eax = &pFont->GetContainer()->operator [](id);

        regs.ecx.i = *(uint32_t *)(regs.ebp.i - 0x30);
    }
};

//CBitmapFont_Destructor
//ClearValues
//8B 04 B7 85 C0 74 0B
//5
//1095476
struct CBitmapFont_Destructor_ClearValues
{
    void operator()(injector::reg_pack &regs) const
    {
        CBitmapCharacterSet *pSet = regs.edi;

        if (pSet->GetContainer() != nullptr)
        {
            delete pSet->GetContainer();
        }

        *pSet->field<InfoContainer *, 0>() = nullptr;

        regs.ef.zero_flag = true;
    }
};

//CBitmapFont_RenderToScreen
//ReadWord
//8A 87 ? ? ? ? 88 86 ? ? ? ? 46
//13
//0x1098CA0
struct CBitmapFont_RenderToScreen_ReadWord
{
    void operator()(injector::reg_pack &regs) const
    {
        g_context.pFont = *(CBitmapFont **)(regs.ebp.i - 0x10);
        g_context.pSet = g_context.pFont->GetCharacterSet();

        g_context.textLength = *(uint32_t *)(regs.ebp.i - 0x14);
        g_context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

        Functions::GetTwoUnicode(
            g_game.pOriginalText,
            regs.edi.i,
            g_context.textLength,
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            g_context.useSpecialChars);

        regs.eax.i = g_context.unicode;
        utf8::append(g_context.unicode, g_game.pWord + regs.esi.i);
        regs.esi.i += g_context.unicodeLength;
        regs.edi.i += (g_context.unicodeLength - 1);
        *(uint32_t *)(regs.ebp.i - 0x30) = regs.edi.i;

        if (!Functions::IsLatin1Char(g_context.unicode))
        {
            regs.ecx = 0;
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
    void operator()(injector::reg_pack &regs) const
    {
        regs.eax = g_context.unicode;

        regs.ecx = g_context.pFont->GetCharacterValue(g_context.unicode);
    }
};

//CBitmapFont_RenderToScreen
//Delim
//66 83 79 06 00
//5
//0x1098F0A
struct CBitmapFont_RenderToScreen_Delim
{
    void operator()(injector::reg_pack &regs) const
    {
        EU4CharInfo *pInfo = regs.ecx;

        if (pInfo->h == 0 || !Functions::IsLatin1Char(g_context.unicode))
        {
            regs.ef.zero_flag = true;
        }
        else
        {
            regs.ef.zero_flag = false;
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
    void operator()(injector::reg_pack &regs) const
    {
        g_context.textLength = *(uint32_t *)(regs.ebp.i - 0x30);
        g_context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

        Functions::GetTwoUnicode(
            g_game.pText,
            regs.edi.i,
            g_context.textLength,
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            g_context.useSpecialChars);

        regs.edx = g_context.pSet;
        regs.ecx = g_context.unicode;
        regs.eax = g_context.unicode;

        regs.edi.i += (g_context.unicodeLength - 1);

        regs.esi = g_context.pFont->GetCharacterValue(g_context.unicode);
    }
};

//CBitmapFont_RenderToTexture
//AppendChar
//8A 04 30 88 85 60 FF FF FF FF B5 60 FF FF FF E8
//20
//0x1096BBE
struct CBitmapFont_RenderToTexture_AppendChar
{
    void operator()(injector::reg_pack &regs) const
    {
        CString *pRight = (CString *)(regs.ebp.i - 0x64);

        //Initialize
        pRight->_sso_head[0] = 0;
        pRight->_length = 0;
        pRight->_capacity = 15;

        char *pSrc = (char *)regs.eax + regs.esi.i;
        char *pDst = pRight->_sso_head;

        ptrdiff_t length = utf8::internal::sequence_length(pSrc);

        strncpy(pDst, pSrc, length);
        pDst[length] = 0;
        pRight->_length = length;

        regs.eax = pRight;
    }
};

//CBitmapFont_RenderToTexture
//GetCharInfo1
//8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00
//13
//0x1096FF9
struct CBitmapFont_RenderToTexture_GetCharInfo1
{
    void operator()(injector::reg_pack &regs) const
    {
        g_context.pFont = regs.edi;
        g_context.pSet = g_context.pFont->GetCharacterSet();


        g_context.textLength = *(uint32_t *)(regs.ebp.i - 0x28);
        g_context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

        Functions::GetTwoUnicode(
            regs.eax,
            regs.esi.i,
            g_context.textLength,
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            g_context.useSpecialChars);

        regs.esi.i += (g_context.unicodeLength - 1);

        regs.eax = g_context.pFont->GetCharacterValue(g_context.unicode);
    }
};

//CBitmapFont_RenderToTexture
//Delim
//8B 45 D0 66 83 78 06 00
//5
//0x10970A4
struct CBitmapFont_RenderToTexture_Delim
{
    void operator()(injector::reg_pack &regs) const
    {
        EU4CharInfo *pInfo = regs.eax;

        if (pInfo->h == 0 || !Functions::IsLatin1Char(g_context.unicode))
        {
            regs.ef.zero_flag = true;
        }
        else
        {
            regs.ef.zero_flag = false;
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
    void operator()(injector::reg_pack &regs) const
    {
        g_context.textLength = *(uint32_t *)(regs.ebp.i - 0x38);
        g_context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

        Functions::GetTwoUnicode(
            regs.eax,
            regs.edi.i,
            g_context.textLength,
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            g_context.useSpecialChars);

        regs.edi.i += (g_context.unicodeLength - 1);
        *(uint32_t *)(regs.ebp.i - 0x10) = regs.edi.i;

        regs.eax = g_context.pFont->GetCharacterValue(g_context.unicode);
    }
};

//CBitmapFont_FillVertexBuffer
//ReadChar1
//8A 04 38 8D 4D 80
//17
//0x109CC5C
struct CBitmapFont_FillVertexBuffer_ReadChar1
{
    void operator()(injector::reg_pack &regs) const
    {
        CString *pRight = (CString *)(regs.ebp.i - 0x80);
        CString *pText = regs.esi;

        //Initialize
        pRight->_sso_head[0] = 0;
        pRight->_length = 0;
        pRight->_capacity = 15;

        Functions::GetTwoUnicode(
            regs.eax,
            regs.edi.i,
            pText->length(),
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            false);

        utf8::append(g_context.unicode, pRight->_sso_head);
        pRight->_sso_head[g_context.unicodeLength] = 0;
        pRight->_length = g_context.unicodeLength;

        regs.eax = pRight;
    }
};

//CBitmapFont_FillVertexBuffer
//GetCharInfo1
//0F B6 04 38 8B 94 81 B4 00 00 00
//11
//0x109CCAB
struct CBitmapFont_FillVertexBuffer_GetCharInfo1
{
    void operator()(injector::reg_pack &regs) const
    {
        CBitmapFont *pFont = regs.ecx;
        regs.edx = pFont->GetCharacterValue(g_context.unicode);
        regs.edi.i += (g_context.unicodeLength - 1);
    }
};

//CBitmapFont_FillVertexBuffer
//Delim
//66 83 7A 06 00 0F 85 99 01 00 00
//5
//0x109CD53
struct CBitmapFont_FillVertexBuffer_Delim
{
    void operator()(injector::reg_pack &regs) const
    {
        EU4CharInfo *pInfo = regs.edx;

        if (pInfo->h == 0 || !Functions::IsLatin1Char(g_context.unicode))
        {
            regs.ef.zero_flag = true;
        }
        else
        {
            regs.ef.zero_flag = false;
        }
    }
};

//CBitmapFont_FillVertexBuffer
//GetCharInfo2
//0F B6 04 30 8B 3C 83
//7
//0x109CFC9
struct CBitmapFont_FillVertexBuffer_GetCharInfo2
{
    void operator()(injector::reg_pack &regs) const
    {
        CString *pString = (CString *)(regs.ebp.i - 0x50);
        CBitmapCharacterSet *pSet = regs.ebx;

        Functions::GetTwoUnicode(
            regs.eax,
            regs.esi.i,
            pString->length(),
            g_context.unicode,
            g_context.unicodeLength,
            g_context.nextUnicode,
            false);

        regs.esi.i += (g_context.unicodeLength - 1);

        regs.edi = pSet->GetCharacterValue(g_context.unicode);
    }
};

//CurveText
//USAGE
//PATTERN
//NOPLENGTH
//ADDRESS
struct CurveText_USAGE
{
    void operator()(injector::reg_pack &regs) const
    {

    }
};

int __fastcall CBitmapFont::GetWidthOfString(CBitmapFont * pFont, int, const char * Text, const int nLength, bool bUseSpecialChars)
{
    static const float fIconWidth = 8.0f;

    char Tag[128];
    float vTempWidth = 0.0f; //当前行宽度
    int nWidth = 0; //最大行宽度

    CBitmapCharacterSet *pSet = pFont->GetCharacterSet();

    int nRealSize = nLength;

    if (nLength < 0)
    {
        nRealSize = strlen(Text);
    }

    std::vector<uint32_t> wideText;
    utf8::utf8to32(Text, Text + nRealSize, back_inserter(wideText));

    for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
    {
        uint32_t unicode = *strit;

        if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA4 || unicode == 0xA7))
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
            else if (unicode == 0xA3)
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
            else if (unicode == 0xA4)
            {
                vTempWidth += fIconWidth;
            }
            else if (unicode == 0xA7)
            {
                ++strit;
            }
        }
        else
        {
            const EU4CharInfo *pValues = pFont->GetCharacterValue(unicode);

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
                vTempWidth += pValues->xadvance * pSet->GetScaleX();

                if (pValues->h == 0 || !Functions::IsLatin1Char(unicode))
                {
                    nWidth = max(nWidth, vTempWidth);
                }
            }
        }
    }

    return max(nWidth, vTempWidth);
}

int __fastcall CBitmapFont::GetHeightOfString(CBitmapFont * pFont, int, const CString *text, int nMaxWidth, int nMaxHeight, const CVector2<int> *BorderSize, bool bUseSpecialChars)
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
    CBitmapCharacterSet *pSet = pFont->GetCharacterSet();

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

    std::vector<uint32_t> wideText;
    utf8::utf8to32(text_view.begin(), text_view.end(), back_inserter(wideText));

    for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
    {
        uint32_t unicode = *strit;

        if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA7))
        {
            if (unicode == 0xA3)
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
            else if (unicode == 0xA7)
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

            pValues = pFont->GetCharacterValue(unicode);

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
                float fWidth = pValues->xadvance * pFont->GetCharacterSet()->GetScaleX();
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

int __fastcall CBitmapFont::GetActualRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, CVector2<short> *NeededSize, bool bUseSpecialChars)
{
    static const float fIconWidth = 8.0f;

    CBitmapCharacterSet *pSet = pFont->GetCharacterSet();

    char Tag[128];
    int nLinesHeight[110] = { 0 };
    int nLines = 0;
    int nWidth = 0;
    int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScaleX();
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

        std::vector<uint32_t> wideText;
        utf8::utf8to32(source_view.begin(), source_view.end(), std::back_inserter(wideText));

        for (auto strit = wideText.begin(); strit < wideText.end() && nLines != 52; ++strit)
        {
            uint32_t unicode = *strit;

            if (unicode == 0xA7)
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
            else if (unicode == 0xA3)
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
                const EU4CharInfo *pValues = pFont->GetCharacterValue(unicode);

                if (pValues)
                {
                    float fWidth = pValues->xadvance * pSet->GetScaleX();
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

void __fastcall CBitmapFont::GetRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bUseSpecialChars)
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

    CBitmapCharacterSet *pSet = pFont->GetCharacterSet();

    std::string Result{ OriginalText->c_str() };

    std::vector<uint32_t> wideText;
    utf8::utf8to32(Result.begin(), Result.end(), std::back_inserter(wideText));

    int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScaleX();
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
        uint32_t unicode = *strit;

        if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA7))
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
            else if (unicode == 0xA3)
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
            else if (unicode == 0xA7)
            {
                ++strit;
            }
        }
        else
        {
            const EU4CharInfo *pValues = pFont->GetCharacterValue(unicode);

            if (pValues)
            {
                float fWidth = pValues->xadvance * pSet->GetScaleX();
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
                            utf8::utf32to8(wideText.begin(), strit - 4, back_inserter(Result));
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
                            utf8::utf32to8(wideText.begin(), strit - 4, back_inserter(Result));
                            Result += " ...";
                        }
                        else
                        {
                            Result.clear();
                            utf8::utf32to8(wideText.begin(), strit, back_inserter(Result));
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

void __fastcall CBitmapFont::GetActualRealRequiredSizeActually(CBitmapFont *pFont, int, const CString *Text, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bWholeWordOnly, bool bAddBreaksToNewText, bool bUseSpecialChars)
{
    static const float fIconWidth = 8.0f;

    //edi Text
    //ebx NewText
    //xmm2 vWordWidth
    //xmm3 vTempWidth
    //xmm4 vHeight

    char Tag[128];

    CBitmapCharacterSet *pSet = pFont->GetCharacterSet();

    int nDefaultLineHeight = pFont->get_field<int, 0x4D4>() * pSet->GetScaleX();
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

    std::vector<uint32_t> wideText;
    std::string_view source_view{ Text->c_str() };
    utf8::utf8to32(source_view.begin(), source_view.end(), std::back_inserter(wideText));

    std::vector<uint32_t>::iterator nLastIndexOfWholeWord = wideText.begin();
    std::vector<uint32_t>::iterator nCurrentLineStartIndex = wideText.begin();

    if (Text->length() != 0)
    {
        for (auto strit = wideText.begin(); strit < wideText.end(); ++strit)
        {
            auto unicode = *strit;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA7))
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
                else if (unicode == 0xA3)
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
                else if (unicode == 0xA7)
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

                const EU4CharInfo *pValues = pFont->GetCharacterValue(unicode);

                if (pValues)
                {
                    float fWidth = pValues->xadvance * pSet->GetScaleX();
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
                                utf8::utf32to8(nCurrentLineStartIndex, nLastIndexOfWholeWord, back_inserter(Result));
                                Result += '\n';
                                nCurrentLineStartIndex = nLastIndexOfWholeWord + 1;
                            }
                            else
                            {
                                utf8::utf32to8(nCurrentLineStartIndex, strit, back_inserter(Result));
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
                                    utf8::utf32to8(wideText.begin(), nLastIndexOfWholeWord, back_inserter(Result));
                                }
                                else
                                {
                                    if (strit - wideText.begin() > 4)
                                    {
                                        utf8::utf32to8(wideText.begin(), strit, back_inserter(Result));
                                    }
                                    else
                                    {
                                        utf8::utf32to8(wideText.begin(), strit - 4, back_inserter(Result));
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
                            utf8::utf32to8(nCurrentLineStartIndex, strit, back_inserter(Result));
                            Result += '\n';

                            nCurrentLineStartIndex = strit + 1;
                        }

                        if (vHeight > nMaxHeight)
                        {
                            if (!bAddBreaksToNewText)
                            {
                                if (!bWholeWordOnly && (strit - wideText.begin()) > 4)
                                {
                                    utf8::utf32to8(wideText.begin(), strit - 4, std::back_inserter(Result));
                                    Result += " ...";
                                }
                                else
                                {
                                    utf8::utf32to8(wideText.begin(), strit, std::back_inserter(Result));
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
        utf8::utf32to8(nCurrentLineStartIndex, wideText.end(), std::back_inserter(Result));
        NewText->assign(Result.c_str());
    }
    else
    {
        NewText->assign(Text->c_str());
    }
}

void CBitmapFont::InitAndPatch()
{
    //去掉Kerning
    injector::MakeNOP(g_pattern.find_first("50 56 57 8D 89 B4 00 00 00 E8 ? ? ? ? E9").integer(), 14);

    //整个换掉的函数
    injector::MakeJMP(g_pattern.find_first("81 EC 8C 00 00 00 53 8B 5D 0C").integer(-6), GetWidthOfString);
    injector::MakeJMP(g_pattern.find_first("81 EC AC 00 00 00 8B 55 0C").integer(-6), GetHeightOfString);
    injector::MakeJMP(g_pattern.find_first("81 EC 0C 03 00 00 8B 45 0C").integer(-0x18), GetActualRequiredSize);
    injector::MakeJMP(g_pattern.find_first("81 EC CC 00 00 00 53 56 57 8B 7D 08 89 4D F0").integer(-0x18), GetRequiredSize);
    injector::MakeJMP(g_pattern.find_first("81 EC 04 01 00 00 53 8B 5D 0C 56").integer(-0x18), GetActualRealRequiredSizeActually);

    //ParseFontFile
    g_pattern.find_pattern("6A 10 E8 ? ? ? ? 8B 4D D0 83 C4 04");
    injector::MakeInline<CBitmapFont_ParseFontFile_AllocValue>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(13));

    g_pattern.find_pattern("89 84 8A B4 00 00 00");
    injector::MakeNOP(g_pattern.get_first().pointer(), 7);

    //Destructor
    g_pattern.find_pattern("8B 04 B7 85 C0 74 0B");
    injector::MakeInline<CBitmapFont_Destructor_ClearValues>(g_pattern.get_first().pointer());

    //RenderToScreen
    //0x1098610

    g_pattern.find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
    injector::MakeInline<CBitmapFont_RenderToScreen_ReadWord>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(13));

    g_pattern.find_pattern("0F B6 C0 8B 0C 82");
    injector::MakeInline<CBitmapFont_RenderToScreen_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(9));

    g_pattern.find_pattern("66 83 79 06 00");
    injector::MakeInline<CBitmapFont_RenderToScreen_Delim>(g_pattern.get_first().pointer());

    g_pattern.find_pattern("8A 8F ? ? ? ? 8B 55 CC");
    injector::MakeNOP(g_pattern.get_first().pointer(20), 3);
    injector::MakeInline<CBitmapFont_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(12));

    
    //RenderToTexture
    //0x1096A60

    g_pattern.find_pattern("8A 04 30 88 85 60 FF FF FF FF B5 60 FF FF FF E8");
    injector::MakeInline<CBitmapFont_RenderToTexture_AppendChar>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(20));

    g_pattern.find_pattern("8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00");
    injector::MakeInline<CBitmapFont_RenderToTexture_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(13));

    g_pattern.find_pattern("8B 45 D0 66 83 78 06 00");
    injector::MakeInline<CBitmapFont_RenderToTexture_Delim>(g_pattern.get_first().pointer(3));

    g_pattern.find_pattern("0F B6 04 38 8B 04 81");
    injector::MakeInline<CBitmapFont_RenderToTexture_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));

    //FillVertexBuffer
    //0x109CB80
    g_pattern.find_pattern("8A 04 38 8D 4D 80");
    injector::MakeInline<CBitmapFont_FillVertexBuffer_ReadChar1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(17));

    g_pattern.find_pattern("0F B6 04 38 8B 94 81 B4 00 00 00");
    injector::MakeInline<CBitmapFont_FillVertexBuffer_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(11));

    g_pattern.find_pattern("66 83 7A 06 00 0F 85 99 01 00 00");
    injector::MakeInline<CBitmapFont_FillVertexBuffer_Delim>(g_pattern.get_first().pointer());

    g_pattern.find_pattern("0F B6 04 30 8B 3C 83");
    injector::MakeInline<CBitmapFont_FillVertexBuffer_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));
}
