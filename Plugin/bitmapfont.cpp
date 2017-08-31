#include "stdinc.h"
#include "plugin.h"
#include "bitmapfont.h"
#include "functions.h"
#include "eu4.h"
#include "cjk_fonts.h"

namespace BitmapFont
{
    

    static std::vector<uint32_t> wbuffer;

    static CJKFont *coop_font;
    static uint32_t code_point;
    static uint32_t next_code_point;
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

    int GetWidthOfString(CBitmapFont * pFont, int edx, const char * text, const int length, bool bUseSpecialChars)
    {
        static const float fIconWidth = 8.0f;

        char tag[128];
        float vTempWidth = 0.0f;
        int nWidth = 0;

        CJKFont *cjk = CSingleton<CJKFontManager>::Instance().GetFont(pFont->GetFontPath());

        int real_length = length;

        if (length < 0)
        {
            real_length = strlen(text);
        }

        utf8::unchecked::iterator<const char *> u8it{ text };
        utf8::unchecked::iterator<const char *> u8end{ text + real_length };

        while (u8it.base() < u8end.base())
        {
            uint32_t unicode = *u8it;

            if (bUseSpecialChars && (unicode == 0x40 || unicode == 0x7B || unicode == 0xA3 || unicode == 0xA4 || unicode == 0xA7))
            {
                switch (unicode)
                {
                case 0x40:
                    std::advance(u8it, 3);
                    vTempWidth += injector::thiscall<int(CBitmapFont *)>::vtbl<30>(pFont);
                    break;

                case 0x7B:
                    std::advance(u8it, 2);
                    vTempWidth += fIconWidth;
                    break;

                case 0xA3:
                    ++u8it;
                    memset(tag, 0, 128);

                    {
                        size_t tag_len = 0;
                        uint32_t tag_char = *u8it;

                        while (Functions::IsTextIconChar(tag_char))
                        {
                            tag[tag_len] = tag_char;
                            ++tag_len;
                            ++u8it;
                            tag_char = *u8it;
                        }
                    }

                    vTempWidth += injector::thiscall<int(CBitmapFont *, const char *)>::vtbl<28>(pFont, tag);
                    break;

                case 0xA4:
                    vTempWidth += fIconWidth;
                    break;

                case 0xA7:
                    ++u8it;
                    break;

                default:
                    break;
                }
            }
            else
            {

            }

            ++u8it;
        }

        return nWidth;
    }

    void InitAndPatch()
    {

    }
}
