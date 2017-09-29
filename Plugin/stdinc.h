#pragma once

#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <array>
#include <type_traits>
#include <vector>
#include <tuple>
#include <utility>
#include <map>
#include <memory>
#include <unordered_map>
#include <string>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <string_view>
#include <functional>
#include <utility>
#include <cstring>
#include <cctype>
#include <optional>
#include <filesystem>
#include <d3dx9.h>
#include <sstream>
#include "utf8cpp/utf8.h"
//#include "eu4utf8/eu4utf8.h"
#include "byte_pattern.h"
#include "injector/hooking.hpp"
#include "injector/calling.hpp"
#include "injector/assembly.hpp"

#define VALIDATE_SIZE(type,size) static_assert(sizeof(type)==size);

struct IncompleteClass
{
    template <typename T, std::uintptr_t offset>
    T *field()
    {
        return (T *)(reinterpret_cast<std::uintptr_t>(this) + offset);
    }

    template <typename T, std::uintptr_t offset>
    T get_field()
    {
        return *(T *)(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
};
