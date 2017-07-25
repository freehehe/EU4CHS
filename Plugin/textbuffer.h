#pragma once
#include "stdinc.h"
#include "eu4.h"

struct CTextBuffer: IncompleteClass
{
	CString *field_1C()
	{
		return field<CString, 0x1C>();
	}

	static void Patch();
};
