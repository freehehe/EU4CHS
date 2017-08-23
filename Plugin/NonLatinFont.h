#pragma once
#include "stdinc.h"
#include "eu4.h"

class NonLatinFont
{
public:
    struct CharacterValues
    {
        EU4CharacterValues _value;
        std::uint16_t _page;
    };

    static const std::uint32_t invalid_replacement = L'？';

    NonLatinFont();

    void InitWithFile(const std::experimental::filesystem::path &fntname);
    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    std::int16_t GetKerning(std::uint32_t first, std::uint32_t second);
    CharacterValues *GetValue(std::uint32_t unicode);
    TextureGFX *GetTexture(std::uint32_t unicode);

    void GeneratePrimitivesDX9(std::uint32_t unicode, const CRect<uint16_t> *dstRect, std::uint32_t color);
    void DrawAllDX9();

private:
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;
    std::unordered_map<std::uint32_t, CharacterValues> _values;
    std::unordered_map<std::uint64_t, std::int16_t> _kernings;
    std::vector<TextureGFX> _textures;
    std::vector<std::string> _texturenames;
    std::experimental::filesystem::path _workingdir;
    std::vector<std::vector<EU4Vertex>> _vertices;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
    void SetKernings();
};
