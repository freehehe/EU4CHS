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
#include <unordered_map>
#include <string>
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
