#include "eu4.h"
#include "byte_pattern.h"
#include "../include/injector/calling.hpp"

void *eu4_malloc(std::size_t size)
{
	static void *pfunction = g_pattern.set_pattern("55 8B EC EB 1F").force_search().get(0).pointer();

	return injector::cstd<void *(std::size_t)>::call(pfunction, size);
}

void eu4_free(void *p, int a2, int a3)
{
	static void *pfunction = g_pattern.set_pattern("55 8B EC 83 C8 FF 8B 4D 0C 33 D2 F7 75 10 3B C8 0F 87 A6 C9 F7 00").force_search().get(0).pointer();

	injector::cstd<void(void *, int, int)>::call(pfunction, p, a2, a3);
}
