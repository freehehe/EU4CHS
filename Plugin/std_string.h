#pragma once
#include "stdinc.h"
#include "eu4.h"

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

    size_t length() const
    {
        return _length;
    }

    CString &assign(const char *cstr)
    {
        return injector::thiscall<CString &(CString *, const char *, size_t)>::call(g_game.pfCString_Assign, this, cstr, strlen(cstr));
    }

    const char *c_str() const
    {
        return _capacity > 15 ? _heap : _sso_head;
    }
};
VALIDATE_SIZE(CString, 0x18)
