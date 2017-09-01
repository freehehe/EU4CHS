#pragma once

#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <utility>
#include <map>
#include <unordered_map>
#include <string>
#include <array>
#include <iterator>
#include <algorithm>
#include <string_view>
#include <functional>
#include <utility>
#include <cstring>
#include <cctype>
#include <optional>
#include <filesystem>
#include <d3dx9.h>
#include <sstream>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/hooking.hpp"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"

template <typename T>
class CSingleton
{
public:
    static T &Instance()
    {
        static T _instance;

        return _instance;
    }
};

class memory_pointer
{
    union
    {
        void *_pointer;
        std::uintptr_t _address;
    };

public:
    explicit memory_pointer(void *pointer)
        : _pointer(pointer)
    {
    }

    explicit memory_pointer(std::uintptr_t address)
        : _address(address)
    {
    }

    memory_pointer(const memory_pointer &rhs) = default;

    std::uintptr_t address(std::ptrdiff_t offset = 0) const
    {
        return (this->_address + offset);
    }

    template<typename T = void>
    T *pointer(std::ptrdiff_t offset = 0) const
    {
        return reinterpret_cast<T *>(this->address(offset));
    }

    bool operator==(const memory_pointer &rhs) const
    {
        return this->address() == rhs.address();
    }

    bool operator!=(const memory_pointer &rhs) const
    {
        return this->address() != rhs.address();
    }

    bool operator>(const memory_pointer &rhs) const
    {
        return this->address() > rhs.address();
    }

    bool operator<(const memory_pointer &rhs) const
    {
        return this->address() < rhs.address();
    }

    bool operator>=(const memory_pointer &rhs) const
    {
        return this->address() >= rhs.address();
    }

    bool operator<=(const memory_pointer &rhs) const
    {
        return this->address() <= rhs.address();
    }

    memory_pointer operator+(std::ptrdiff_t offset) const
    {
        return memory_pointer{ this->address(offset) };
    }

    memory_pointer operator-(std::ptrdiff_t offset) const
    {
        return memory_pointer{ this->address(-offset) };
    }

    memory_pointer &operator+=(std::ptrdiff_t offset)
    {
        this->_address += offset;
        return *this;
    }

    memory_pointer &operator-=(std::ptrdiff_t offset)
    {
        this->_address -= offset;
        return *this;
    }

    operator std::uintptr_t() const
    {
        return this->address();
    }

    template <typename T>
    operator T*() const
    {
        return this->pointer<T>();
    }
};
