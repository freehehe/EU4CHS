#pragma once
#include "stdinc.h"
#include "eu4.h"

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
    static const std::uint32_t invalid_replacement = L'艚';

    NonLatinFont();

    void InitWithFile(const std::experimental::filesystem::path &fntname);
    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    std::int16_t GetKerning(std::uint32_t first, std::uint32_t second);
    EU4CharacterValues *GetValue(std::uint32_t unicode);
    TextureGFX *GetTexture(std::uint32_t unicode);
};

