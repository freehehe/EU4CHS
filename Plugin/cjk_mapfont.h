#pragma once
#include "stdinc.h"
#include "eu4.h"

class CJKMapFont
{
public:
    struct CharacterValues
    {
        EU4CharacterValues EU4Values;
        std::uint16_t PageIndex;
    };

    static const std::uint32_t invalid_replacement = L'？';

    CJKMapFont(const std::experimental::filesystem::path &fntname);

    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    const CharacterValues *GetValue(std::uint32_t unicode);

    void AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, SProvinceTextVertex *pVertices);
    void DrawAllDX9();

private:
    bool _initialized;
    std::experimental::filesystem::path _workingdir;
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;

    std::array<std::unique_ptr<CharacterValues>, 0x10000> _values;

    std::vector<LPDIRECT3DTEXTURE9> _textures;
    std::vector<std::string> _texturenames;
    std::vector<std::vector<SProvinceTextVertex>> _vertices;
    std::vector<std::vector<UINT>> _indices;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
};
