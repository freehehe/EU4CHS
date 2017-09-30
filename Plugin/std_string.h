#pragma once
#include "stdinc.h"

class CString
{
    union
    {
        char *_heap;
        char _sso_head[4];
    };

    char _sso_rest[12];
    size_t _length;
    size_t _capacity;

public:
    CString() = delete;
    CString(const CString &) = delete;
    CString &operator=(const CString &) = delete;

    size_t length() const;
    CString &assign(const char *cstr);
    const char *c_str() const;
};
VALIDATE_SIZE(CString, 0x18)
