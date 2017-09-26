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

    const CharacterValues *GetValue(std::uint32_t unicode);

    void AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, STextVertex *pVertices);

    void DrawAllDX9();

protected:
    bool _initialized;
    std::experimental::filesystem::path _workingdir;
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;
    std::uint16_t _pages;

    std::array<std::unique_ptr<CharacterValues>, 0x10000> _values;

    std::vector<LPDIRECT3DTEXTURE9> _textures;
    std::vector<std::string> _texturenames;
    std::vector<std::vector<STextVertex>> _vertices;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
};
