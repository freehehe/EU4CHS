#include "std_string.h"
#include "functions.h"

size_t CString::length() const
{
    return _length;
}

CString &CString::assign(const char *cstr)
{
    return injector::thiscall<CString &(CString *, const char *, size_t)>::call(Functions::pfCString_Assign, this, cstr, strlen(cstr));
}

const char *CString::c_str() const
{
    return _capacity > 15 ? _heap : _sso_head;
}
