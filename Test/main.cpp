#include <iostream>
#include <cstring>
#include <cctype>
#include <iterator>
#include "../include/utf8cpp/utf8.h"

using namespace std;

uint32_t GetNextUnicode(const char *pText, bool bUseSpecialChars)
{
    uint32_t next = utf8::unchecked::peek_next(pText);

    if (bUseSpecialChars)
    {
        while (next == 0xA7)
        {
            utf8::unchecked::advance(pText, 2);
            next = utf8::unchecked::peek_next(pText);
        }
    }

    return utf8::unchecked::peek_next(pText);
}

int main()
{
    const char *u8test = u8"1§YGCC§!是神圣罗马帝国的官方信仰";
    const char *pText = u8test;

    cout << hex << showbase;

    while (utf8::unchecked::peek_next(pText) != 0)
    {
        const char *pText2 = pText;

        uint32_t first, next;

        first = utf8::unchecked::next(pText2);
        next = GetNextUnicode(pText2, true);

        cout << first << endl;
        cout << next << endl;

        utf8::unchecked::advance(pText, 1);
    }

    return 0;
}
