#include "stdinc.h"
#include "functions.h"
#include "eu4.h"
#include "hook_variables.h"
#include "byte_pattern.h"

using namespace std;

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest)
    {
        string_view source_view(source);
        g_context.wideText.clear();
        utf8::utf8to32(source_view.begin(), source_view.end(), back_inserter(g_context.wideText));

        for (uint32_t &cp : g_context.wideText)
        {
            switch (cp)
            {
            case 0x80:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
            case 0x88:
            case 0x89:
            case 0x8a:
            case 0x8b:
            case 0x8c:
            case 0x8e:
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97:
            case 0x98:
            case 0x99:
            case 0x9a:
            case 0x9b:
            case 0x9c:
            case 0x9e:
            case 0x9f:
                cp = '?';
                break;

            case 0xA3:
            case 0xA4:
            case 0xA7:
                cp -= 0xA0;
                break;

            case 0x152: cp = 0x8c; break;
            case 0x153: cp = 0x9c; break;
            case 0x160: cp = 0x8a; break;
            case 0x161: cp = 0x9a; break;
            case 0x178: cp = 0x9f; break;
            case 0x17d: cp = 0x8e; break;
            case 0x17e: cp = 0x9e; break;
            case 0x192: cp = 0x83; break;
            case 0x2c6: cp = 0x88; break;
            case 0x2dc: cp = 0x98; break;
            case 0x2013: cp = 0x96; break;
            case 0x2014: cp = 0x97; break;
            case 0x2018: cp = 0x91; break;
            case 0x2019: cp = 0x92; break;
            case 0x201a: cp = 0x82; break;
            case 0x201c: cp = 0x93; break;
            case 0x201d: cp = 0x94; break;
            case 0x201e: cp = 0x84; break;
            case 0x2020: cp = 0x86; break;
            case 0x2021: cp = 0x87; break;
            case 0x2022: cp = 0x95; break;
            case 0x2026: cp = 0x85; break;
            case 0x2030: cp = 0x89; break;
            case 0x2039: cp = 0x8b; break;
            case 0x203a: cp = 0x9b; break;
            case 0x20ac: cp = 0x80; break;
            case 0x2122: cp = 0x99; break;

            default:
                break;
            }
        }

        g_context.wideText.push_back(0);
        utf8::utf32to8(g_context.wideText.begin(), g_context.wideText.end(), dest);
    }

    void ConvertLatin1ToUTF8(char * source)
    {
        string_view source_view{ source };

        g_context.wideText.clear();

        transform(source_view.begin(), source_view.end(), back_inserter(g_context.wideText), 
            [](char c) {
            return *(unsigned char *)&c;
        });

        g_context.wideText.push_back(0);

        utf8::unchecked::utf32to8(g_context.wideText.begin(), g_context.wideText.end(), source);
    }

    bool IsLatin1Char(uint32_t cp)
    {
        return cp <= 0xFF;
    }

    bool IsTextIconChar(uint32_t cp)
    {
        return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
    }

    //Points after first character
    uint32_t GetNextUnicode(const char *pText, bool bUseSpecialChars)
    {
        uint32_t next = utf8::unchecked::peek_next(pText);

        if (bUseSpecialChars)
        {
            while (next == 0x7)
            {
                utf8::unchecked::advance(pText, 2);
                next = utf8::unchecked::peek_next(pText);
            }
        }

        return utf8::unchecked::peek_next(pText);
    }

    void ConvertSpecialChars(char *source)
    {
        string_view source_view(source);

        if (all_of(source_view.begin(), source_view.end(), isascii))
        {
            return;
        }

        g_context.wideText.clear();
        utf8::utf8to32(source_view.begin(), source_view.end(), back_inserter(g_context.wideText));

        for (uint32_t &cp : g_context.wideText)
        {
            switch (cp)
            {
            case 0xA3:
            case 0xA4:
            case 0xA7:
                cp -= 0xA0;
                break;

            default:
                break;
            }
        }

        g_context.wideText.push_back(0);
        utf8::utf32to8(g_context.wideText.begin(), g_context.wideText.end(), source);
    }

    struct CReader_ReadSimpleStatement_0x161_7
    {
        void operator()(injector::reg_pack *regs) const
        {
            CToken *pSrcToken = (CToken *)(regs->esi);
            CToken *pDstToken = (CToken *)(regs->edi);

            if (pSrcToken->_LexerToken == 0xF)
            {
                ConvertSpecialChars(pSrcToken->_szVal);
            }

            *pDstToken = *pSrcToken;
        }
    };
    
    struct CSdlEvents_HandlePdxEvents_0x2DE
    {
        void operator()(injector::reg_pack *regs) const
        {
            CInputEvent temp;
            char *pText = (char *)(regs->ebp - 0x48);

            string_view text_view(pText);

            for (char c : text_view)
            {
                temp.Init(c);

                injector::thiscall<void(uint32_t, const CInputEvent *)>::vtbl<3>(regs->ebx, &temp);
            }

            memset(pText, 0, 32);

            regs->ecx = 0;
            regs->eax = 0;
        }
    };

    void InitAndPatch()
    {
        //yml转码函数
        injector::MakeJMP(g_pattern.find_pattern("81 EC B0 00 00 00 53 56 8B F1 8B DA").get(0).integer(-0x18), ConvertUTF8ToLatin1);

        //转换txt文本中的A3A4A7
        g_pattern.find_pattern("C3 8B 73 1C 8D BB 30 04 00 00 83 C6 0C B9 82 00 00 00 F3 A5"); //mov ecx, 0x82; rep movsd
        injector::MakeInline<CReader_ReadSimpleStatement_0x161_7>(g_pattern.get(0).integer(13), g_pattern.get(0).integer(20));

        //从输入法接受整个字符串
        g_pattern.find_pattern("8B 4D B8 32 C0"); //mov ecx, [ebp - 0x48]; xor al, al
        injector::MakeInline<CSdlEvents_HandlePdxEvents_0x2DE>(g_pattern.get(0).integer());

        //WriteVariable 0xA7
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("C6 06 A7").get(0).integer(2), 7, true);
        injector::WriteMemory<uint8_t>(g_pattern.find_pattern("66 C7 06 A7 21").get(0).integer(3), 7, true);
    }
}
