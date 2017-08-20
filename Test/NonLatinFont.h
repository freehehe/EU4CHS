#pragma once
#include <filesystem>
#include <d3dx9.h>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string_view>
#include <string>
#include <memory>
#include <optional>
#include <utility>

struct EU4CharValues
{
    std::uint16_t _x;
    std::uint16_t _y;
    std::uint16_t _w;
    std::uint16_t _h;
    std::uint16_t _xoff;
    std::uint16_t _yoff;
    std::uint16_t _xadvance;
    bool _kerning;
};

struct CharValues
{
    EU4CharValues _value;
    std::uint8_t _page;
};

struct TextureGFX
{
    LPDIRECT3DTEXTURE9 field_0 = nullptr;
    bool field_4 = false;
    DWORD field_8 = 0;
    int field_C = 0;
    int field_10 = 0;
    int field_14 = 0;
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

class NonLatinFont
{
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;
    std::unordered_map<std::uint32_t, CharValues> _values;
    std::unordered_map<std::uint64_t, std::int16_t> _kernings;
    std::vector<TextureGFX> _textures;
    std::vector<std::string> _texturenames;
    std::experimental::filesystem::path _workingdir;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
    void SetKernings();

public:
    static const std::uint32_t invalid_replacement = L'？';

    void InitWithFile(const std::experimental::filesystem::path &fntname);
    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    std::int16_t GetKerning(std::uint32_t first, std::uint32_t second) const;
    const EU4CharValues *GetValue(std::uint32_t unicode) const;
};

