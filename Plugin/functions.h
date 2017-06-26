#pragma once
#include "stdinc.h"
#include "../include/injector/assembly.hpp"

class CGlobalFunctions
{
public:
	static void __fastcall ConvertUTF8ToLatin1(const char *, char *);
	static void Patch();
};

