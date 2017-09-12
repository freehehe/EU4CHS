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
    const char *u8test = u8"哈1哈§YGC主义GC§!是神圣罗马帝国的官方信仰。只有信仰§YGC主义GC§!的君主才有资格成为皇帝，并且所有的选帝侯必须也信仰§YGC主义GC§!。皇帝可以通过“宗教统一”外交选项逼迫异端成员国转信官方信仰。";
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
