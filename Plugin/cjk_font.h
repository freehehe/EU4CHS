#pragma once
#include "stdinc.h"
#include "eu4.h"

class CJKFontBase
{
public:
    struct CJKCharInfo
    {
        EU4CharInfo Value;
        uint16_t Page;
    };

    static const uint32_t INVALID_REPLACEMENT = L'？';

    const CJKCharInfo &GetValue(uint32_t unicode);
    const EU4CharInfo *GetEU4Value(uint32_t unicode);

    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    virtual ~CJKFontBase() = default;

protected:
    CJKFontBase(const std::experimental::filesystem::path &fntname);

    std::uint16_t _TextureWidth;
    std::uint16_t _TextureHeight;
    std::uint16_t _PageCount;

    std::vector<std::pair<std::string, LPDIRECT3DTEXTURE9>> _Textures;

private:
    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);

    std::unordered_map<uint32_t, CJKCharInfo> _Values;

    std::experimental::filesystem::path _WorkingDir;
    std::vector<char> _PagesBlock;
};
