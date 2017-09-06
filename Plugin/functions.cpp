#include "stdinc.h"
#include "functions.h"
#include "eu4.h"
#include "byte_pattern.h"
#include "hook_variables.h"

using namespace std;

namespace Functions
{
    void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest)
    {
        string_view source_view(source);
        hook_context.wideText.clear();
        utf8::utf8to32(source_view.begin(), source_view.end(), back_inserter(hook_context.wideText));

        for (uint32_t &cp : hook_context.wideText)
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

        hook_context.wideText.push_back(0);
        utf8::utf32to8(hook_context.wideText.begin(), hook_context.wideText.end(), dest);
    }

    struct CReader_ReadSimpleStatement_0x161_7
    {
        void operator()(injector::reg_pack *regs) const
        {
            //esi+4 char *source
            //copy 520 bytes from [esi] to [edi]
        }
    };

    void ConvertSpecialChars(char *source)
    {
        string_view source_view(source);
        hook_context.wideText.clear();
        utf8::utf8to32(source_view.begin(), source_view.end(), back_inserter(hook_context.wideText));

        for (uint32_t &cp : hook_context.wideText)
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

        hook_context.wideText.push_back(0);
        utf8::utf32to8(hook_context.wideText.begin(), hook_context.wideText.end(), source);
    }

    bool IsNativeChar(uint32_t cp)
    {
        return cp <= 0xFF;
    }

    bool IsTextIconChar(uint32_t cp)
    {
        return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
    }

    bool IsSpecialChar(uint32_t cp)
    {
        return (cp == 0x40 || cp == 0x7B || cp == 0x7D || cp == 0xA3 || cp == 0xA4 || cp == 0xA7);
    }

    void InitAndPatch()
    {
        injector::MakeJMP(g_pattern.find_pattern("81 EC B0 00 00 00 53 56 8B F1 8B DA").get(0).integer(-0x18), ConvertUTF8ToLatin1);

        g_pattern.find_pattern("B9 82 00 00 00 F3 A5");
        injector::MakeInline<CReader_ReadSimpleStatement_0x161_7>(g_pattern.get(0).integer(), g_pattern.get(0).integer(7));
        
    }
}
