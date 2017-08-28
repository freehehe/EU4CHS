#include "stdinc.h"
#include "byte_pattern.h"

void FuckThem()
{
    //cmp reg8, 0xA?
    g_pattern.find_pattern("80 F? A?"); 
    g_pattern.for_each_result(
        [](memory_pointer p) 
    {
        injector::WriteMemory<uint8_t>(p.pointer<uint8_t>(2), 3, true);
    });

    g_pattern.find_pattern("A4");
    g_pattern.for_each_result(
        [](memory_pointer p)
    {
        injector::WriteMemory<uint8_t>(p.pointer<uint8_t>(2), 4, true);
    });

    g_pattern.find_pattern("A7");
    g_pattern.for_each_result(
        [](memory_pointer p)
    {
        injector::WriteMemory<uint8_t>(p.pointer<uint8_t>(2), 7, true);
    });
}