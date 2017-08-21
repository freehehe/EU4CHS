#include <iostream>
#include "NonLatinFont.h"

int main()
{
    NonLatinFont font;
    font.InitWithFile("normal.bin");

    std::cout << font.GetKerning(L'哈', L'蛤') << std::endl;
    std::cout << font.GetValue(NonLatinFont::invalid_replacement)->xadvance << std::endl;

    return 0;
}
