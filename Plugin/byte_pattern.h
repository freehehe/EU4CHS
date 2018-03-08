//Core code from Hooking.Patterns
//https://github.com/ThirteenAG/Hooking.Patterns

#pragma once
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include <utility>

extern HMODULE pattern_default_module;

class memory_pointer
{
    union
    {
        void *Pointer;
        std::uintptr_t Address;
    };

public:
    memory_pointer()
        :Pointer{}
    {

    }

    memory_pointer(void *pointer)
        : Pointer(pointer)
    {
    }

    memory_pointer(std::uintptr_t address)
        : Address(address)
    {
    }

    memory_pointer(const memory_pointer &rhs) = default;

    std::uintptr_t i(std::ptrdiff_t offset = 0) const
    {
        return (this->Address + offset);
    }

    template<typename T = void>
    T *p(std::ptrdiff_t offset = 0) const
    {
        return reinterpret_cast<T *>(this->i(offset));
    }

    operator std::uintptr_t() const
    {
        return this->i();
    }

    template <typename T>
    operator T*() const
    {
        return this->p<T>();
    }
};

class byte_pattern
{
    std::vector<std::pair<std::uintptr_t, std::uintptr_t>> _ranges;
    std::vector<std::uint8_t> _pattern;
    std::vector<std::uint8_t> _mask;
    std::vector<memory_pointer> _results;
    std::string _literal;

    std::ptrdiff_t _bmbc[256];

    void transform_pattern(const char *pattern_literal);
    void get_module_ranges(memory_pointer module);

    void bm_preprocess();
    void bm_search();

    void debug_output() const;

public:
    byte_pattern();

    byte_pattern &set_pattern(const char *pattern_literal);

    byte_pattern &set_module();
    byte_pattern &set_module(memory_pointer module);
    byte_pattern &set_range(memory_pointer beg, memory_pointer end);
    byte_pattern &search();

    byte_pattern &find_pattern(const char *pattern_literal);

    memory_pointer get(std::size_t index) const;
    memory_pointer get_first() const;

    std::size_t count() const;
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
