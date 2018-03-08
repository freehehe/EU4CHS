#pragma once
#include "stdinc.h"
#include "eu4.h"
#include "std_string.h"

class CTextBuffer :public IncompleteClass
{
public:
    CString *GetString();
    static void InitAndPatch();
};
