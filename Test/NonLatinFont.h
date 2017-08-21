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

#define VALIDATE_SIZE(type,size) static_assert(sizeof(type)==size);

typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;
typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

struct EU4CharacterValues
{
    int16 x;
    int16 y;
    int16 w;
    int16 h;
    int16 xoff;
    int16 yoff;
    int16 xadvance;
    bool kerning;
};
VALIDATE_SIZE(EU4CharacterValues, 0x10)

struct TextureGFX
{
    LPDIRECT3DTEXTURE9 field_0 = nullptr;
    bool field_4 = false;
    DWORD field_8 = 0;
    int field_C = 0;
    int field_10 = 0;
    int field_14 = 0;
};

VALIDATE_SIZE(TextureGFX, 0x18)
class NonLatinFont
{
    struct CharacterValues
    {
        EU4CharacterValues _value;
        std::uint16_t _page;
    };

    union UnicodeCharPair
    {
        struct
        {
            std::uint32_t _first;
            std::uint32_t _second;
        };

        std::uint64_t _packed;
    };

    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;
    std::vector<std::pair<std::uint32_t, CharacterValues>> _values;
    std::vector<std::pair<std::uint64_t, std::int16_t>> _kernings;
    std::vector<TextureGFX> _textures;
    std::vector<std::string> _texturenames;
    std::experimental::filesystem::path _workingdir;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
    void SetKernings();
    void SortData();

public:
    static const std::uint32_t invalid_replacement = L'？';

    NonLatinFont();

    void InitWithFile(const std::experimental::filesystem::path &fntname);
    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    std::int16_t GetKerning(std::uint32_t first, std::uint32_t second);
    EU4CharacterValues *GetValue(std::uint32_t unicode);
    TextureGFX *GetTexture(std::uint32_t unicode);
};

auto qq = sizeof(NonLatinFont);
