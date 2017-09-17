#include <iostream>
#include <cstdint>
#include <cstring>
#include "../include//utf8cpp/utf8.h"

using namespace std;

void GetTwoUnicode(const char * pText, uint32_t index, uint32_t text_length, uint32_t &first, uint32_t &second, ptrdiff_t &length, bool bUseSpecialChars)
{
    second = 0;

    //First
    if (index < text_length)
    {
        length = utf8::internal::sequence_length(pText);
        first = utf8::unchecked::peek_next(pText + index);
        index += length;
    }

    //Second
    if (index < text_length)
    {
        ptrdiff_t second_length = utf8::internal::sequence_length(pText + index);
        uint32_t temp_second = utf8::unchecked::peek_next(pText + index);
        index += second_length;

        if (bUseSpecialChars)
        {
            while (temp_second == 0x7)
            {
                index += 2;

                if (index >= text_length)
                {
                    return;
                }

                temp_second = utf8::unchecked::peek_next(pText + index);
            }
        }

        second = temp_second;
    }
}

int main()
{
    uint32_t first, second;
    ptrdiff_t length;
    const char pText[] = u8"哈a哈\x7Y哈\x7!a哈\x3mifa";

    GetTwoUnicode(pText, 0, strlen(pText), first, second, length, true);

    cout << first << second << endl;

    return 0;
}
