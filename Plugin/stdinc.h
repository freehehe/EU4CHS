#pragma once
#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>
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
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/hooking.hpp"
#include "../include/injector/calling.hpp"
#include "../include/injector/assembly.hpp"

#define VALIDATE_SIZE(type,size) static_assert(sizeof(type)==size);

typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;
typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

struct IncompleteClass
{
	template <typename T, std::uintptr_t offset>
	T *field()
	{
		return (T *)((std::uintptr_t)this + offset);
	}
};
