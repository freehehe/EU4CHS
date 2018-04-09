#include "bitmapfont.h"
#include "misc.h"
#include "eu4.h"
#include "byte_pattern.h"

namespace BitmapFont
{
    static char *pOriginalText;
    static char *pWord;
    static char *pText;

    ValuesContainer *CBitmapCharacterSet::GetContainer()
    {
        if (get_field<ValuesContainer *, 0>() == nullptr)
        {
            *field<ValuesContainer *, 0>() = new ValuesContainer;
        }

        return get_field<ValuesContainer *, 0>();
    }

    SBitmapCharacterValue *CBitmapCharacterSet::GetCharacterValue(std::uint32_t character)
    {
        auto *container = GetContainer();

        auto it = container->find(character);

        if (it != container->end())
        {
            return &it->second;
        }
        else
        {
            if (Misc::IsLatin1Char(character))
                return nullptr;
            else
                return &container->find(L'¿')->second;
        }
    }

    CBitmapCharacterSet *CBitmapFont::GetCharacterSet()
    {
        return field<CBitmapCharacterSet, 0xB4>();
    }

    //------------------------------------------------------------------------------------------------

        //8B 04 B7 85 C0 74 0B 6A 10
        //5
    struct CBitmapCharacterSetDtorHook
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.edi;

            if (pSet->GetContainer())
            {
                delete pSet->GetContainer();
                regs.esi.i = 256;
            }

            regs.ef.zero_flag = true;
        }
    };

    //6a 10 e8 ? ? ? ? 8b 4d d0
    //5 (nop e8 call)
    //89 84 8a b4 00 00 00
    //7
    struct CBitmapFontParseFontFile_AllocValue
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapFont *pFont = *(CBitmapFont **)(regs.ebp.i - 0x10);
            uint32_t character = *(uint32_t *)(regs.ebp.i - 0x14);

            regs.eax.p = &pFont->GetCharacterSet()->GetContainer()->operator[](character);
        }
    };

    //8a 04 30 88 85 60 ff ff ff
    //20
    struct CBitmapFontRenderToTexture_ReadChar
    {
        void operator()(injector::reg_pack &regs) const
        {
            CString *pRight = regs.ecx;

            //初始化右值
            pRight->_length = 0;
            pRight->_capacity = 15;

            char *pSrc = (char *)regs.eax + regs.esi.i;
            char *pDst = pRight->_sso_head;

            ptrdiff_t length = eu4utf8::internal::sequence_length(pSrc);

            strncpy(pDst, pSrc, length);
            pDst[length] = 0;
            pRight->_length = length;

            regs.eax.p = pRight;
        }
    };

    //8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00
    //13
    struct CBitmapFont_RenderToTexture_GetCharInfo1
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapFont *pFont = regs.edi;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

            Misc::GetTwoUnicode(regs.eax, regs.esi.i);
            regs.esi.i += (Misc::context.unicodeLength - 1);

            regs.eax.p = pFont->GetCharacterSet()->GetCharacterValue(Misc::context.unicode);
        }
    };

    //66 83 78 06 00 0F 85
    //5
    struct CBitmapFontRenderToTexture_Delim
    {
        void operator()(injector::reg_pack &regs) const
        {
            SBitmapCharacterValue *pInfo = regs.eax;

            if (pInfo->h == 0 || !Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ef.zero_flag = true;
            }
            else
            {
                regs.ef.zero_flag = false;
            }
        }
    };

    //0F B6 04 38 8B 04 81
    //7
    struct CBitmapFontRenderToTexture_GetCharInfo2
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.ecx;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

            Misc::GetTwoUnicode(regs.eax, regs.edi.i);

            regs.edi.i += (Misc::context.unicodeLength - 1);
            *(uint32_t *)(regs.ebp.i - 0x1C) = regs.edi.i;

            regs.eax.p = pSet->GetCharacterValue(Misc::context.unicode);
        }
    };

    //8A 87 ? ? ? ? 88 86 ? ? ? ? 46
    //13
    struct CBitmapFontRenderToScreen_ReadChar
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapFont *pFont = *(CBitmapFont **)(regs.ebp.i - 0x10);

            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

            Misc::GetTwoUnicode(pOriginalText, regs.edi.i);

            regs.eax.i = Misc::context.unicode;
            eu4utf8::append(Misc::context.unicode, pWord + regs.esi.i);
            regs.esi.i += Misc::context.unicodeLength;
            regs.edi.i += (Misc::context.unicodeLength - 1);
            *(uint32_t *)(regs.ebp.i - 0x24) = regs.edi.i;

            if (!Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ecx.i = 0;
            }
        }
    };

    //0F B6 C0 8B 0C 82
    //6
    struct CBitmapFontRenderToScreen_GetCharInfo1
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = *(CBitmapCharacterSet **)(regs.ebp.i - 0x3C);

            regs.eax.i = Misc::context.unicode;
            regs.ecx.p = pSet->GetCharacterValue(Misc::context.unicode);
        }
    };

    //66 83 79 06 00
    //5
    struct CBitmapFontRenderToScreen_Delim
    {
        void operator()(injector::reg_pack &regs) const
        {
            SBitmapCharacterValue *pInfo = regs.ecx;

            if (pInfo->h == 0 || !Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ef.zero_flag = true;
            }
            else
            {
                regs.ef.zero_flag = false;
            }
        }
    };

    //8B B4 87 B4 00 00 00
    //7
    struct CBitmapFontRenderToScreen_GetCharInfo2
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapFont *pFont = regs.edi;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x3C);

            Misc::GetTwoUnicode(pText, regs.edx.i);

            regs.edx.i += (Misc::context.unicodeLength - 1);

            regs.esi.p = pFont->GetCharacterSet()->GetCharacterValue(Misc::context.unicode);
        }
    };

    //0F B6 04 30 8B 0C 82
    //7
    struct CBitmapFontGetHeightOfString_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.edx;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x18);

            Misc::GetTwoUnicode(regs.eax, regs.esi.i);

            regs.ecx.p = pSet->GetCharacterValue(Misc::context.unicode);
            regs.esi.i += (Misc::context.unicodeLength - 1);
        }
    };

    //80 3C 30 20 0F 85 CC FD FF FF
    //10
    __declspec(naked) void CBitmapFontGetHeightOfString_Delim()
    {
        __asm
        {
            pop ecx;

            cmp Misc::context.unicode, 0x20;
            jz back;

            cmp Misc::context.unicode, 0xFF;
            jg back;

            add ecx, -0x22F;

        back:
            jmp ecx;
        }
    }

    //0F B6 C2 8B 3C 81
    //6
    struct CBitmapFontGetWidthOfString_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.ecx;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x10);

            Misc::GetTwoUnicode(regs.edx, regs.esi.i);

            regs.edx.i = Misc::context.unicode;
            regs.edi.p = pSet->GetCharacterValue(Misc::context.unicode);
            regs.esi.i += (Misc::context.unicodeLength - 1);
        }
    };

    //66 83 7F 06 00
    //5
    struct CBitmapFontGetWidthOfString_Delim
    {
        void operator()(injector::reg_pack &regs) const
        {
            SBitmapCharacterValue *pInfo = regs.edi;

            if (pInfo->h == 0 || !Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ef.zero_flag = true;
            }
            else
            {
                regs.ef.zero_flag = false;
            }
        }
    };

    //----------------------------------------------------------------------------------------------
    //FillVertexBuffer

    //8A 04 38 8D 4D 80
    //17
    struct CBitmapFontFillVertexBuffer_ReadChar1
    {
        void operator()(injector::reg_pack &regs) const
        {
            CString *pRight = (CString *)(regs.ebp.i - 0x80);
            CString *pText = regs.esi;

            Misc::context.useSpecialChars = false;

            //Initialize
            pRight->_sso_head[0] = 0;
            pRight->_length = 0;
            pRight->_capacity = 15;

            Misc::GetTwoUnicode(regs.eax, regs.edi.i);

            eu4utf8::append(Misc::context.unicode, pRight->_sso_head);
            pRight->_sso_head[Misc::context.unicodeLength] = 0;
            pRight->_length = Misc::context.unicodeLength;

            regs.eax.p = pRight;
        }
    };

    //8B 94 81 B4 00 00 00
    //7
    struct CBitmapFontFillVertexBuffer_GetCharInfo1
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapFont *pFont = regs.ecx;
            regs.edx.p = pFont->GetCharacterSet()->GetCharacterValue(Misc::context.unicode);
            regs.edi.i += (Misc::context.unicodeLength - 1);
        }
    };

    //66 83 7A 06 00 0F 85 DB 01 00 00
    //5
    struct CBitmapFontFillVertexBuffer_Delim
    {
        void operator()(injector::reg_pack &regs) const
        {
            BitmapFont::SBitmapCharacterValue *pInfo = regs.edx;

            if (pInfo->h == 0 || !Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ef.zero_flag = true;
            }
            else
            {
                regs.ef.zero_flag = false;
            }
        }
    };

    //0F B6 04 30 8B 3C 83
    //7
    struct CBitmapFontFillVertexBuffer_GetCharInfo2
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.ebx;

            Misc::GetTwoUnicode(regs.eax, regs.esi.i);

            regs.esi.i += (Misc::context.unicodeLength - 1);

            regs.edi.p = pSet->GetCharacterValue(Misc::context.unicode);
        }
    };
    
    //----------------------------------------------------------------------------------------------

    //8B 0C 81 8B 45 D8 89 4D D0
    //6
    struct CBitmapFontGetActualRequiredSize_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            CString *pString = regs.esi;
            CBitmapCharacterSet *pSet = regs.ecx;

            Misc::context.useSpecialChars = true;

            Misc::GetTwoUnicode(pString->c_str(), regs.edi.i);

            regs.eax.p = pSet;
            regs.ecx.p = pSet->GetCharacterValue(Misc::context.unicode);
            regs.edi.i += (Misc::context.unicodeLength - 1);
        }
    };

    //0F BF 41 06 0F 57 C9
    //7
    __declspec(naked) void CBitmapFontGetActualRequiredSize_Delim()
    {

        __asm
        {
            movsx eax, word ptr[ecx + 6];
            xorps xmm1, xmm1;
            pop ecx;

            cmp Misc::context.unicode, 0xFF;
            jle back;
            xor eax, eax;

        back:
            jmp ecx;
        }
    };

    //8A 04 30 0F B6 C0 8B 14 82
    //9
    struct CBitmapFontGetRequiredSize_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            CBitmapCharacterSet *pSet = regs.edx;
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x1C);

            Misc::GetTwoUnicode(regs.eax, regs.esi.i);

            regs.edx.p = pSet->GetCharacterValue(Misc::context.unicode);
            regs.esi.i += (Misc::context.unicodeLength - 1);
        }
    };

    //8A 04 30 3C 20
    //5
    struct CBitmapFontGetActualRequiredSizeActually_IsSpace
    {
        void operator()(injector::reg_pack &regs) const
        {
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x24);
            Misc::GetTwoUnicode(regs.eax, regs.esi.i);

            if (Misc::context.unicode == 0x20 || !Misc::IsLatin1Char(Misc::context.unicode))
            {
                regs.ef.zero_flag = true;
            }
            else
            {
                regs.ef.zero_flag = false;
            }
        }
    };

    //8A 04 30 8B 55 14 0F B6 C0 8B 14 82
    //12
    struct CBitmapFontGetActualRequiredSizeActually_GetCharInfo
    {
        void operator()(injector::reg_pack &regs) const
        {
            Misc::context.useSpecialChars = *(bool *)(regs.ebp.i + 0x24);
            CBitmapCharacterSet *pSet = *(CBitmapCharacterSet **)(regs.ebp.i + 0x14);

            Misc::GetTwoUnicode(regs.eax, regs.esi.i);
            regs.esi.i += (Misc::context.unicodeLength - 1);
            regs.edx.p = pSet->GetCharacterValue(Misc::context.unicode);
        }
    };

    void InitAndPatch()
    {
        //~BitmapCharacterSet
        g_pattern.find_pattern("8B 04 B7 85 C0 74 0B 6A 10");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapCharacterSetDtorHook>(g_pattern.get_first().address());

        //ParseFontFile
        g_pattern.find_pattern("6a 10 e8 ? ? ? ? 8b 4d d0");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontParseFontFile_AllocValue>(g_pattern.get_first().address(2));

        g_pattern.find_pattern("89 84 8a b4 00 00 00");
        if (g_pattern.has_size(1))
            injector::MakeNOP(g_pattern.get_first().address(), 7);

        //RenderToTexture
        g_pattern.find_pattern("8a 04 30 88 85 60 ff ff ff");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToTexture_ReadChar>(g_pattern.get_first().address(), g_pattern.get_first().address(20));

        g_pattern.find_pattern("8A 04 30 0F B6 C0 8B 84 87 B4 00 00 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFont_RenderToTexture_GetCharInfo1>(g_pattern.get_first().address(), g_pattern.get_first().address(13));

        g_pattern.find_pattern("66 83 78 06 00 0F 85");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToTexture_Delim>(g_pattern.get_first().address());

        g_pattern.find_pattern("0F B6 04 38 8B 04 81");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToTexture_GetCharInfo2>(g_pattern.get_first().address(), g_pattern.get_first().address(7));

        //RenderToScreen
        g_pattern.find_pattern("68 00 0C 00 00 50 68");
        if (g_pattern.has_size(1))
            pOriginalText = *g_pattern.get_first().pointer<char *>(7);

        g_pattern.find_pattern("68 3C 0C 00 00 6A 00 68");
        if (g_pattern.has_size(1))
            pText = *g_pattern.get_first().pointer<char *>(8);

        g_pattern.find_pattern("68 00 01 00 00 6A 00 68");
        if (g_pattern.has_size(1))
            pWord = *g_pattern.get_first().pointer<char *>(8);

        g_pattern.find_pattern("8A 87 ? ? ? ? 88 86 ? ? ? ? 46");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToScreen_ReadChar>(g_pattern.get_first().address(), g_pattern.get_first().address(13));

        g_pattern.find_pattern("0F B6 C0 8B 0C 82");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToScreen_GetCharInfo1>(g_pattern.get_first().address(), g_pattern.get_first().address(6));

        g_pattern.find_pattern("66 83 79 06 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToScreen_Delim>(g_pattern.get_first().address());

        g_pattern.find_pattern("8B B4 87 B4 00 00 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontRenderToScreen_GetCharInfo2>(g_pattern.get_first().address(), g_pattern.get_first().address(7));

       //GetHeightOfString
        g_pattern.find_pattern("0F B6 04 30 8B 0C 82");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetHeightOfString_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(7));

        g_pattern.find_pattern("80 3C 30 20 0F 85 CC FD FF FF");
        if (g_pattern.has_size(1))
        {
            injector::MakeNOP(g_pattern.get_first().address(), 10);
            injector::MakeCALL(g_pattern.get_first().address(), CBitmapFontGetHeightOfString_Delim);
        }
        //GetWidthOfString
        g_pattern.find_pattern("0F B6 C2 8B 3C 81");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetWidthOfString_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(6));

        g_pattern.find_pattern("66 83 7F 06 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetWidthOfString_Delim>(g_pattern.get_first().address());

        //FillVertexBuffer
        g_pattern.find_pattern("8A 04 38 8D 4D 80");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontFillVertexBuffer_ReadChar1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(17));

        g_pattern.find_pattern("8B 94 81 B4 00 00 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontFillVertexBuffer_GetCharInfo1>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));

        g_pattern.find_pattern("66 83 7A 06 00 0F 85 DB 01 00 00");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontFillVertexBuffer_Delim>(g_pattern.get_first().pointer());

        g_pattern.find_pattern("0F B6 04 30 8B 3C 83");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontFillVertexBuffer_GetCharInfo2>(g_pattern.get_first().pointer(), g_pattern.get_first().pointer(7));

        //GetActualRequiredSize
        g_pattern.find_pattern("8B 0C 81 8B 45 D8 89 4D D0");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetActualRequiredSize_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(6));

        g_pattern.find_pattern("0F BF 41 06 0F 57 C9");
        if (g_pattern.has_size(1))
        {
            injector::MakeNOP(g_pattern.get_first().address(), 7);
            injector::MakeCALL(g_pattern.get_first().address(), CBitmapFontGetActualRequiredSize_Delim);
        }

        //GetRequiredSize
        g_pattern.find_pattern("8A 04 30 0F B6 C0 8B 14 82");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetRequiredSize_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(9));

        //GetActualRealRequiredSizeActually
        g_pattern.find_pattern("8A 04 30 3C 20");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetActualRequiredSizeActually_IsSpace>(g_pattern.get_first().address());

        g_pattern.find_pattern("8A 04 30 8B 55 14 0F B6 C0 8B 14 82");
        if (g_pattern.has_size(1))
            injector::MakeInline<CBitmapFontGetActualRequiredSizeActually_GetCharInfo>(g_pattern.get_first().address(), g_pattern.get_first().address(12));
    }
}
