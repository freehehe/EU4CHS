#include <iostream>
#include <filesystem>
#include <cstring>
#include <cctype>
#include "../include/utf8cpp/utf8.h"

using namespace std;
using namespace std::experimental;

bool IsTextIconChar(uint32_t cp)
{
    return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
}

uint32_t GetNextUnicode(const char *pText, bool bUseSpecialChars)
{
    utf8::unchecked::iterator<const char *> strit{ pText };

    uint32_t first = *strit;

    if (bUseSpecialChars)
    {
        if (first == 0x3)
        {
            ++strit;

            size_t index = 0;

            while (IsTextIconChar(*strit) && (index < 127))
            {
                ++index;
                ++strit;
            }
        }
        else if (first == 0x7)
        {
            ++strit;
        }
        else if (first == 0x40)
        {
            std::advance(strit, 3);
        }
        else if (first == 0x7B)
        {
            std::advance(strit, 2);
        }
    }

    ++strit;
    return *strit;
}

int main()
{
    const char *u8test=u8""
    return 0;
}
