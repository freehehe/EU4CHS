#include "stdinc.h"
#include "cjk_font.h"
#include "functions.h"

using namespace std;
using namespace std::experimental;

union UnicodeCharPair
{
    struct
    {
        std::uint32_t _first;
        std::uint32_t _second;
    };

    std::uint64_t _packed;
};

CJKFont::CJKFont(const std::experimental::filesystem::path & fntname)
{
    _initialized = false;
    _values.reserve(30000);

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
#pragma pack(push,1)
    struct BinaryKerningValue
    {
        uint32_t first;
        uint32_t second;
        int16_t value;
    };
#pragma pack(pop)

    std::vector<BinaryKerningValue> kernings_block;

    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    kernings_block.resize(block_size / sizeof(BinaryKerningValue));
    fread(kernings_block.data(), block_size, 1, file);

    for (auto &binary : kernings_block)
    {
        UnicodeCharPair cp;

        cp._first = binary.first;
        cp._second = binary.second;

        _kernings.emplace(cp._packed, binary.value);
    }
}

void CJKFont::SetKernings()
{
    for (auto &kp : _kernings)
    {
        UnicodeCharPair cp;

        cp._packed = kp.first;

        _values.find(cp._first)->second.EU4Values.kerning = true;
    }
}

std::int16_t CJKFont::GetKerning(uint32_t first, uint32_t second) const
{
    UnicodeCharPair duochar;

    duochar._first = first;
    duochar._second = second;

    auto it = _kernings.find(duochar._packed);

    if (it == _kernings.end())
    {
        return 0;
    }
    else
    {
        return it->second;
    }
}

void CJKFont::InitWithFile(const std::experimental::filesystem::path &fntname)
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

    SetKernings();

    _initialized = true;
}

void CJKFont::LoadTexturesDX9()
{
    for (auto &name : _texturenames)
    {
        TextureGFX gfx;

        D3DXCreateTextureFromFileW(game_meta.pDX9Device, (_workingdir / name).c_str(), &gfx.field_0);

        _textures.emplace_back(gfx);
    }

    _vertices.clear();
    _vertices.resize(_textures.size());
}

void CJKFont::UnloadTexturesDX9()
{
    for (auto &texture : _textures)
    {
        texture.field_0->Release();
    }

    _textures.clear();
}

CJKFont::CharacterValues *CJKFont::GetValue(uint32_t unicode)
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

TextureGFX * CJKFont::GetTexture(std::uint32_t unicode)
{
    uint16_t page;

    auto it = _values.find(unicode);

    if (it != _values.end())
    {
        page = it->second.PageIndex;
    }
    else
    {
        page = _values.find(invalid_replacement)->second.PageIndex;
    }

    return &_textures[page];
}

void CJKFont::SetPrimitivesDX9(std::uint32_t unicode, const CRect<int> *dstRect, std::uint32_t color)
{
    if (Functions::IsNativeChar(unicode))
    {

    }
    else
    {
        OurVertex vertices[6];

        CharacterValues *pValues = GetValue(unicode);

        CRect<int> screenRect;
        CRect <float> textureRect;

        copy(begin(vertices), end(vertices), back_inserter(_vertices[pValues->PageIndex]));
    }
}

void CJKFont::DrawAllDX9()
{
    LPDIRECT3DDEVICE9 pDevice = game_meta.pDX9Device;

    pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

    for (size_t texture_index = 0; texture_index < _vertices.size(); ++texture_index)
    {
        if (_vertices[texture_index].empty())
        {
            return;
        }

        pDevice->SetTexture(0, _textures[texture_index].field_0);
        pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, _vertices[texture_index].size() / 3, _vertices[texture_index].data(), sizeof(OurVertex));
        _vertices[texture_index].clear();
    }
}
