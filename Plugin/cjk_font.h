#pragma once
#include "stdinc.h"
#include "eu4.h"

class CJKFont
{
public:
    struct CharacterValues
    {
        EU4CharacterValues EU4Values;
        std::uint16_t PageIndex;
    };

    static const std::uint32_t invalid_replacement = L'？';

    CJKFont(const std::experimental::filesystem::path &fntname);

    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    CharacterValues *GetValue(CBitmapFont *pFont, std::uint32_t unicode);
    std::int16_t GetKerning(CBitmapFont *pFont, uint32_t first, uint32_t second) const;
    TextureGFX *GetTexture(CBitmapFont *pFont, std::uint32_t unicode);

    void SetPrimitivesDX9(std::uint32_t unicode, const CRect<int> *dstRect, std::uint32_t color);
    void DrawAllDX9();

private:
    bool _initialized;
    std::experimental::filesystem::path _workingdir;
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;
    std::unordered_map<std::uint32_t, CharacterValues> _values;
    std::unordered_map<std::uint64_t, std::int16_t> _kernings;
    std::vector<TextureGFX> _textures;
    std::vector<std::string> _texturenames;
    std::vector<std::vector<STextVertex>> _vertices;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
    void SetKernings();
};
