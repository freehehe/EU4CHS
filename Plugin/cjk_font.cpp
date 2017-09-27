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
    _Initialized = false;

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

    _TextureWidth = common_block.scaleW;
    _TextureHeight = common_block.scaleH;
    _PageCount = common_block.pages;
}

void CJKFont::ReadPagesBlock(FILE *file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);

    char *buffer = new char[block_size];

    fread(buffer, block_size, 1, file);

    char *p = buffer;

    for (uint16_t count = 0; count < _PageCount; ++count)
    {
        _TextureFileNames.emplace_back(p);
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

        values.TextureIndex = binary.page;
        values.Value.x = binary.x;
        values.Value.y = binary.y;
        values.Value.w = binary.width;
        values.Value.h = binary.height;
        values.Value.xoff = binary.xoff;
        values.Value.yoff = binary.yoff;
        values.Value.xadvance = binary.xadvance;
        values.Value.kerning = false;

        uint32_t unicode = binary.code;

        if (unicode < 0x10000)
        {
            _Values[unicode] = make_unique<CharacterValues>(values);
        }
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

    if (_Initialized)
    {
        return;
    }

    _WorkingDir = fntname.parent_path();

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

    _Vertices.clear();
    _Vertices.resize(_PageCount);
    
    _Initialized = true;
}

void CJKFont::LoadTexturesDX9()
{
    for (auto &name : _TextureFileNames)
    {
        LPDIRECT3DTEXTURE9 gfx;

        D3DXCreateTextureFromFileW(g_game.pDX9Device, (_WorkingDir / name).replace_extension(".dds").c_str(), &gfx);

        _Textures.emplace_back(gfx);
    }
}

void CJKFont::UnloadTexturesDX9()
{
    for (auto &texture : _Textures)
    {
        texture->Release();
    }

    _Textures.clear();
}

const CJKFont::CharacterValues *CJKFont::GetValue(uint32_t unicode)
{
    if (unicode < 0x10000 && _Values[unicode])
    {
        return _Values[unicode].get();
    }
    else
    {
        return _Values[_InvalidCharacter].get();
    }
}

void CJKFont::AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, STextVertex * pVertices)
{
    float width_ratio = (float)pFont->get_field<int, 0x4E8>() / _TextureWidth;
    float height_ratio = (float)pFont->get_field<int, 0x4EC>() / _TextureHeight;

    pVertices[0].UV.x *= width_ratio;
    pVertices[0].UV.y *= height_ratio;
    pVertices[1].UV.x *= width_ratio;
    pVertices[1].UV.y *= height_ratio;
    pVertices[2].UV.x *= width_ratio;
    pVertices[2].UV.y *= height_ratio;
    pVertices[3].UV.x *= width_ratio;
    pVertices[3].UV.y *= height_ratio;
    pVertices[4].UV.x *= width_ratio;
    pVertices[4].UV.y *= height_ratio;
    pVertices[5].UV.x *= width_ratio;
    pVertices[5].UV.y *= height_ratio;

    copy_n(pVertices, 6, back_inserter(_Vertices[GetValue(unicode)->TextureIndex]));
}

void CJKFont::DrawAllDX9()
{
    for (size_t index = 0; index < _PageCount; ++index)
    {
        if (!_Vertices[index].empty())
        {
            g_game.pDX9Device->SetTexture(0, _Textures[index]);
            g_game.pDX9Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, _Vertices[index].size() / 3, _Vertices[index].data(), sizeof(STextVertex));

            _Vertices[index].clear();
        }
    }
}
