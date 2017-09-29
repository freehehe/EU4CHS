#include "cjk_font.h"
#include <iostream>

//£¤§
using namespace std;
// 0 - 10FFFF

int main()
{
    CJKFontBase font{ "C:\\test.fnt" };

    CJKFontBase::CJKCharInfo *p;

    p = font.GetValueHash(L'草');
    p = font.GetValueMap(L'泥');
    p = font.GetValueVec(L'马');

    cout << p << endl;

    return 0;
}
