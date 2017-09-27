#pragma once
#include "stdinc.h"
#include "eu4.h"

class CJKFont
{
public:
    struct CharacterValues
    {
        EU4CharacterValues Value;
        std::uint16_t TextureIndex;
    };

    static const std::uint32_t _InvalidCharacter = L'？';

    CJKFont(const std::experimental::filesystem::path &fntname);

    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    const CharacterValues *GetValue(std::uint32_t unicode);

    void AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, STextVertex *pVertices);

    void DrawAllDX9();

protected:
    bool _Initialized;
    std::experimental::filesystem::path _WorkingDir;
    std::uint16_t _TextureWidth;
    std::uint16_t _TextureHeight;
    std::uint16_t _PageCount;

    std::array<std::unique_ptr<CharacterValues>, 0x10000> _Values;

    std::map<uint32_t, std::vector<STextVertex>> _ScreenVertices; //hash, vertices

    std::vector<LPDIRECT3DTEXTURE9> _Textures;
    std::vector<std::string> _TextureFileNames;
    std::vector<std::vector<STextVertex>> _Vertices;

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
};
