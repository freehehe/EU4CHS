#pragma once
#include "stdinc.h"
#include "cjk_font.h"

struct hook_variables
{
    std::vector<uint32_t> wide_text;
    uint32_t *pwide_text;
    CJKFont *cjk_font;
    uint32_t code_point;
    uint32_t next_code_point;
    ptrdiff_t cp_len;
    ptrdiff_t text_index;
    void *ret_addr;
};

extern hook_variables hook_context;
