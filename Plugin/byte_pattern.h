//Core code from Hooking.Patterns
//https://github.com/ThirteenAG/Hooking.Patterns

#pragma once
#include "stdinc.h"

extern const HMODULE pattern_default_module;

enum class byte_mask :std::uint8_t
{
    WILDCARD,
    HIGH_ONLY,
    LOW_ONLY,
    EXACT
};

class memory_pointer
{
    union
    {
        void *_pointer;
        std::uintptr_t _address;
    };

public:
    memory_pointer(void *pointer)
        : _pointer(pointer)
    {
    }

    memory_pointer(std::uintptr_t address)
        : _address(address)
    {
    }

    std::uintptr_t address(std::ptrdiff_t offset = 0) const
    {
        return (this->_address + offset);
    }

    template<typename T = void>
    T *pointer(std::ptrdiff_t offset = 0) const
    {
        return reinterpret_cast<T *>(this->address(offset));
    }
};

class byte_pattern
{
    std::pair<std::uintptr_t, std::uintptr_t> _range;
    std::vector<std::uint8_t> _pattern;
    std::vector<byte_mask> _mask;
    std::vector<memory_pointer> _results;
    std::string _literal;

    bool _processed = false;

    std::ptrdiff_t _bmbc[256];

    void transform_pattern(const char *pattern_literal);
    void get_module_range(memory_pointer module);

    void do_search();

    void bm_preprocess();
    void bm_search();

public:
    byte_pattern();

    byte_pattern &set_pattern(const char *pattern_literal);
    byte_pattern &set_pattern(const void *data, std::size_t size);
    byte_pattern &set_module(memory_pointer module = pattern_default_module);
    byte_pattern &set_range(memory_pointer beg, memory_pointer end);
    byte_pattern &force_search();

    byte_pattern &find_pattern(const char *pattern_literal);

    const memory_pointer &get(std::size_t index) const;

    std::size_t size() const;
    bool has_size(std::size_t expected) const;
    bool empty() const;
    void clear();

    template <typename Fn>
    void for_each_result(Fn Pr) const
    {
        for (auto &result : this->_results)
        {
            Pr(result);
        }
    }
};

extern byte_pattern g_pattern;
