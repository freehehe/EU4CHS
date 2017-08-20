#include "NonLatinFont.h"
#include <iostream>

int main()
{
    NonLatinFont font;

    font.InitWithFile("C:/font/normal.bin");

    std::cout << font.GetKerning(L'a', L'a') << '\n';
    std::cout << font.GetValue(L'c')->_xadvance << std::endl;
    font.LoadTexturesDX9();
    return 0;
}
