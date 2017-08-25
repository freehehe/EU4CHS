#include "stdinc.h"
#include "byte_pattern.h"
#include <stdexcept>

extern const HMODULE pattern_default_module = GetModuleHandleA(NULL);
byte_pattern g_pattern;

const memory_pointer &byte_pattern::get(std::size_t index) const
{
    if (index >= this->_result.size())
    {
        std::stringstream sstr;

        sstr << "Processing pattern: " << this->_literal << "\nTrying to access index " << index << " but only " << this->_result.size() << " results.\nGame will crash.";

        MessageBoxA(NULL, sstr.str().c_str(), "byte_pattern: too few results.", MB_OK);

        throw std::out_of_range{ "Pattern: results accessing out of range." };
    }

    return this->_result[index];
}

byte_pattern &byte_pattern::set_pattern(const char *pattern_literal)
{
    this->_result.clear();
    this->_processed = false;
    this->transform_pattern(pattern_literal);

    return *this;
}

byte_pattern &byte_pattern::set_pattern(const void *data, std::size_t size)
{
    this->_pattern.assign(reinterpret_cast<const std::uint8_t *>(data), reinterpret_cast<const std::uint8_t *>(data) + size);
    this->bm_preprocess();

    return *this;
}

byte_pattern &byte_pattern::set_module(memory_pointer module)
{
    this->get_module_range(module);

    return *this;
}

byte_pattern &byte_pattern::set_range(memory_pointer beg, memory_pointer end)
{
    this->_range = { beg.address(), end.address() };

    return *this;
}

void byte_pattern::do_search()
{
    if (!this->_processed)
    {
        this->force_search();
    }
}

byte_pattern &byte_pattern::force_search()
{
    this->_result.clear();

    this->_processed = true;

    if (!this->_pattern.empty())
    {
        this->bm_search();
    }

    return *this;
}

void byte_pattern::transform_pattern(const char *pattern_literal)
{
    auto tol = [](char ch) -> uint8_t
    {
        if (ch >= 'A' && ch <= 'F') return uint8_t(ch - 'A' + 10);
        if (ch >= 'a' && ch <= 'f') return uint8_t(ch - 'a' + 10);
        return uint8_t(ch - '0');
    };

    auto is_digit = [](char ch) -> bool
    {
        return (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f') || (ch >= '0' && ch <= '9');
    };

    std::array<char, 2> temp_string{ 0, 0 };

    this->_literal = pattern_literal;

    this->_pattern.clear();

    if (pattern_literal == nullptr)
    {
        return;
    }

    const char *patit = pattern_literal;
    const char *patend = (pattern_literal + std::strlen(pattern_literal) + 1);

    while (patit != patend)
    {
        char ch = *patit;

        if (ch == ' ' || ch == 0)
        {
            if (!temp_string[0] && !temp_string[1])
            {
                continue;
            }
            else if (temp_string[0] == '?' && (temp_string[1] == '?' || temp_string[1] == 0))
            {
                this->_pattern.emplace_back();
            }
            else if (is_digit(temp_string[0]) && is_digit(temp_string[1]))
            {
                this->_pattern.emplace_back((tol(temp_string[0]) << 4) | tol(temp_string[1]));
            }
            else
            {
                this->_pattern.clear();
                return;
            }

            temp_string.fill(0);
        }
        else
        {
            if (temp_string[0] == 0)
            {
                temp_string[0] = ch;
            }
            else if (temp_string[1] == 0)
            {
                temp_string[1] = ch;
            }
            else
            {
                this->_pattern.clear();
                return;
            }
        }

        ++patit;
    }

    this->bm_preprocess();
}

void byte_pattern::get_module_range(memory_pointer module)
{
    static auto getSection = [](const PIMAGE_NT_HEADERS nt_headers, unsigned section) -> PIMAGE_SECTION_HEADER
    {
        return reinterpret_cast<PIMAGE_SECTION_HEADER>(
            (UCHAR*)nt_headers->OptionalHeader.DataDirectory +
            nt_headers->OptionalHeader.NumberOfRvaAndSizes * sizeof(IMAGE_DATA_DIRECTORY) +
            section * sizeof(IMAGE_SECTION_HEADER));
    };

    this->_range.first = module.address();

    PIMAGE_DOS_HEADER dosHeader = module.pointer<IMAGE_DOS_HEADER>();
    PIMAGE_NT_HEADERS ntHeader = module.pointer<IMAGE_NT_HEADERS>(dosHeader->e_lfanew);

    for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
    {
        auto sec = getSection(ntHeader, i);
        auto secSize = sec->SizeOfRawData != 0 ? sec->SizeOfRawData : sec->Misc.VirtualSize;
        if (sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)
            this->_range.second = this->_range.first + sec->VirtualAddress + secSize;

        if ((i == ntHeader->FileHeader.NumberOfSections - 1) && this->_range.second == 0)
            this->_range.second = this->_range.first + sec->PointerToRawData + secSize;
    }
}

void byte_pattern::clear()
{
    _range = { 0,0 };
    this->_pattern.clear();
    this->_result.clear();
    this->_processed = false;
}

std::size_t byte_pattern::size() const
{
    return this->_result.size();
}

bool byte_pattern::has_size(std::size_t expected) const
{
    return (this->_result.size() == expected);
}

bool byte_pattern::empty() const
{
    return this->_result.empty();
}

bool byte_pattern::check_address(std::uintptr_t address) const
{
    return std::equal(this->_pattern.begin(), this->_pattern.end(), reinterpret_cast<const uint8_t *>(address));
}

void byte_pattern::bm_preprocess()
{
    std::ptrdiff_t i, j, c;

    for (std::uint32_t bc = 0; bc < 256; ++bc)
    {
        for (i = this->_pattern.size() - 1; i >= 0; --i)
        {
            if (this->_pattern[i].match(bc))
            {
                break;
            }
        }

        this->_bmbc[bc] = i;
    }

    this->_bmgs.resize(this->_pattern.size());    

    for (i = 0; i < this->_pattern.size() - 1; ++i)
    {
        this->_bmgs[i] = this->_pattern.size();
    }

    this->_bmgs[this->_pattern.size() - 1] = 1;

    for (i = this->_pattern.size() - 1, c = 0; i != 0; --i)
    {
        for (j = 0; j < i; ++j)
        {
            if (std::equal(this->_pattern.begin() + i, this->_pattern.end(), this->_pattern.begin() + j))
            {
                if (j == 0)
                {
                    c = this->_pattern.size() - i;
                }
                else
                {
                    if (this->_pattern[i - 1] != this->_pattern[j - 1])
                    {
                        this->_bmgs[i - 1] = j - 1;
                    }
                }
            }
        }
    }

    for (i = 0; i < this->_pattern.size() - 1; ++i)
    {
        if (this->_bmgs[i] != this->_pattern.size())
        {
            this->_bmgs[i] = this->_pattern.size() - 1 - this->_bmgs[i];
        }
        else
        {
            this->_bmgs[i] = this->_pattern.size() - 1 - i + this->_bmgs[i];

            if (c != 0 && this->_pattern.size() - 1 - i >= c)
            {
                this->_bmgs[i] -= c;
            }
        }
    }
}

void byte_pattern::bm_search()
{
    std::uint8_t *range_begin = reinterpret_cast<std::uint8_t *>(this->_range.first);
    std::uint8_t *range_end = reinterpret_cast<std::uint8_t *>(this->_range.second - this->_pattern.size());

    std::ptrdiff_t index;

    __try
    {
        while (range_begin <= range_end)
        {
            for (index = this->_pattern.size() - 1; index >= 0; --index)
            {
                if (!this->_pattern[index].match(range_begin[index]))
                {
                    break;
                }
            }

            if (index == -1)
            {
                this->_result.emplace_back(range_begin);
                range_begin += this->_pattern.size();
            }
            else
            {
                range_begin += std::max(index - this->_bmbc[range_begin[index]], this->_bmgs[index]);
            }
        }
    }
    __except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {

    }
}
