#include "stdinc.h"
#include "cjk_font.h"
#include "functions.h"

using namespace std;
using namespace std::experimental;

union UnicodeCharPair
{
    struct
    {
        uint32_t _first;
        uint32_t _second;
    };

    uint64_t _packed;
};

CJKFont::CJKFont(const filesystem::path & fntname)
{
    _initialized = false;
    _values.reserve(45000);

    InitWithFile(fntname);
}

void CJKFont::ReadInfoBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFont::ReadCommonBlock(FILE * file)
{
#pragma pack(push,1)
    struct CommonBlock
    {
        uint16_t lineHeight;
        uint16_t base;
        uint16_t scaleW;
        uint16_t scaleH;
        uint16_t pages;

        struct
        {
            uint8_t unused : 7;
            bool packed : 1;
        } flags;

        uint8_t alphaChnl;
        uint8_t redChnl;
        uint8_t greenChnl;
        uint8_t blueChnl;
    };
#pragma pack(pop)

    CommonBlock common_block;

    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fread(&common_block, sizeof(CommonBlock), 1, file);

    _scaleW = common_block.scaleW;
    _scaleH = common_block.scaleH;
    _pages = common_block.pages;
}

void CJKFont::ReadPagesBlock(FILE *file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);

    char *buffer = new char[block_size];

    fread(buffer, block_size, 1, file);

    char *p = buffer;

    for (uint16_t count = 0; count < _pages; ++count)
    {
        _texturenames.emplace_back(p);
        p += (strlen(p) + 1);
    }

    delete[]buffer;
}

void CJKFont::ReadCharsBlock(FILE * file)
{
    struct BinaryCharValues
    {
        uint32_t code;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        int16_t xoff;
        int16_t yoff;
        int16_t xadvance;
        uint8_t page;
        uint8_t chnl;
    };

    std::vector<BinaryCharValues> chars_block;

    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    chars_block.resize(block_size / sizeof(BinaryCharValues));
    fread(chars_block.data(), block_size, 1, file);

    for (auto &binary : chars_block)
    {
        CharacterValues values;

        values.PageIndex = binary.page;
        values.EU4Values.x = binary.x;
        values.EU4Values.y = binary.y;
        values.EU4Values.w = binary.width;
        values.EU4Values.h = binary.height;
        values.EU4Values.xoff = binary.xoff;
        values.EU4Values.yoff = binary.yoff;
        values.EU4Values.xadvance = binary.xadvance;
        values.EU4Values.kerning = false;

        uint32_t unicode = binary.code;

        _values.emplace(unicode, values);
    }
}

void CJKFont::ReadKerningsBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFont::InitWithFile(const filesystem::path &fntname)
{
    unsigned char block_type;

    if (_initialized)
    {
        return;
    }

    _workingdir = fntname.parent_path();

    FILE *iFile = fopen(fntname.string().c_str(), "rb");

    fseek(iFile, 4, SEEK_SET);

    while (fread(&block_type, 1, 1, iFile) != 0)
    {
        switch (block_type)
        {
        case 1:
            ReadInfoBlock(iFile);
            break;

        case 2:
            ReadCommonBlock(iFile);
            break;

        case 3:
            ReadPagesBlock(iFile);
            break;

        case 4:
            ReadCharsBlock(iFile);
            break;

        case 5:
            ReadKerningsBlock(iFile);
            break;

        default:
            return;
        }
    }

    fclose(iFile);

    _buffer.clear();
    _buffer.resize(_pages);

    _initialized = true;
}

void CJKFont::LoadTexturesDX9()
{
    for (auto &name : _texturenames)
    {
        TextureGFX gfx;

        D3DXCreateTextureFromFileW(g_game.pDX9Device, (_workingdir / name).c_str(), &gfx.field_0);

        _textures.emplace_back(gfx);
    }
}

void CJKFont::UnloadTexturesDX9()
{
    for (auto &texture : _textures)
    {
        texture.field_0->Release();
    }

    _textures.clear();
}

const CJKFont::CharacterValues *CJKFont::GetValue(uint32_t unicode)
{
    auto it = _values.find(unicode);

    if (it != _values.end())
    {
        return &it->second;
    }
    else
    {
        return &_values.find(invalid_replacement)->second;
    }
}

TextureGFX * CJKFont::GetTexture(uint32_t unicode)
{
    return &_textures[GetValue(unicode)->PageIndex];
}

void CJKFont::AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, STextVertex * pVertices)
{
    float width_ratio = (float)pFont->get_field<int, 0x4E8>() / _scaleW;
    float height_ratio = (float)pFont->get_field<int, 0x4EC>() / _scaleH;

    pVertices[0].UV.x *= width_ratio;
    pVertices[0].UV.y *= width_ratio;
    pVertices[1].UV.x *= width_ratio;
    pVertices[1].UV.y *= width_ratio;
    pVertices[2].UV.x *= width_ratio;
    pVertices[2].UV.y *= width_ratio;
    pVertices[3].UV.x *= width_ratio;
    pVertices[3].UV.y *= width_ratio;
    pVertices[4].UV.x *= width_ratio;
    pVertices[4].UV.y *= width_ratio;
    pVertices[5].UV.x *= width_ratio;
    pVertices[5].UV.y *= width_ratio;

    copy_n(pVertices, 6, back_inserter(_buffer[GetValue(unicode)->PageIndex]));
}

void CJKFont::DrawAllDX9()
{
    for (size_t index = 0; index < _pages; ++index)
    {
        if (!_buffer[index].empty())
        {
            g_game.pDX9Device->SetTexture(0, _textures[index].field_0);
            g_game.pDX9Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
            g_game.pDX9Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, _buffer[index].size() / 3, _buffer[index].data(), sizeof(STextVertex));

            _buffer[index].clear();
        }
    }
}
