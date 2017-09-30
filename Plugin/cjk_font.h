#pragma once
#include "eu4.h"
#include "bitmapfont.h"

class CJKFont
{
public:
    struct CJKCharInfo
    {
        EU4CharInfo Value;
        uint16_t Page;
    };

    static const uint32_t INVALID_REPLACEMENT = L'？';

    CJKCharInfo *GetValue(uint32_t unicode);
    EU4CharInfo *GetEU4Value(uint32_t unicode);

    void LoadTexturesDX9();
    void UnloadTexturesDX9();

    CJKFont(const std::experimental::filesystem::path &fntname);

    void AddVerticesDX9(CBitmapFont *pFont, uint32_t unicode, STextVertex *pVertex);

    //未缓存的绘制
    //直接绘制已有缓存
    //首次绘制并缓存
    void DrawUnbufferedDX9();
    void DrawBufferedDX9(uint32_t hash);
    void DrawAndBufferDX9(uint32_t hash);

private:
    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);

    std::experimental::filesystem::path _WorkingDir;
    std::vector<char> _PagesBlock;

    std::uint16_t _TextureWidth;
    std::uint16_t _TextureHeight;
    std::uint16_t _PageCount;

    std::array<std::unique_ptr<CJKCharInfo>, 0x10000> _Values;
    std::vector<std::pair<std::string, LPDIRECT3DTEXTURE9>> _Textures;
    std::vector<std::vector<STextVertex>> _BaseBuffer;
    std::map<uint32_t, std::vector<std::vector<STextVertex>>> _ScreenBuffer;
    std::vector<std::vector<SProvinceTextVertex>> _ProvinceBuffer;
};

constexpr auto aa = sizeof(std::optional<CJKFont::CJKCharInfo>);
