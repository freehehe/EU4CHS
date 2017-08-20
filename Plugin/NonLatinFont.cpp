#include "stdinc.h"
#include "NonLatinFont.h"

using namespace std;
using namespace std::experimental;

void NonLatinFont::ReadInfoBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void NonLatinFont::ReadCommonBlock(FILE * file)
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

void NonLatinFont::ReadPagesBlock(FILE *file)
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

void NonLatinFont::ReadCharsBlock(FILE * file)
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

        values._page = binary.page;
        values._value.x = binary.x;
        values._value.y = binary.y;
        values._value.w = binary.width;
        values._value.h = binary.height;
        values._value.xoff = binary.xoff;
        values._value.yoff = binary.yoff;
        values._value.xadvance = binary.xadvance;

        values._value.kerning = false;

        uint32_t unicode = binary.code;

        _values.emplace(unicode, values);
    }
}

void NonLatinFont::ReadKerningsBlock(FILE * file)
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

void NonLatinFont::SetKernings()
{
    for (auto &kp : _kernings)
    {
        UnicodeCharPair cp;

        cp._packed = kp.first;

        _values.find(cp._first)->second._value.kerning = true;
    }
}

void NonLatinFont::InitWithFile(const std::experimental::filesystem::path &fntname)
{
    unsigned char block_type;

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
}

void NonLatinFont::LoadTexturesDX9()
{
    _textures.clear();

    for (auto &name : _texturenames)
    {
        TextureGFX gfx;

        D3DXCreateTextureFromFileA(NULL, (_workingdir / name).string().c_str(), &gfx.field_0);

        _textures.emplace_back(gfx);
    }
}

void NonLatinFont::UnloadTexturesDX9()
{
    for (auto &texture : _textures)
    {
        texture.field_0->Release();
    }
}

std::int16_t NonLatinFont::GetKerning(uint32_t first, uint32_t second) const
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

const EU4CharacterValues * NonLatinFont::GetValue(uint32_t unicode) const
{
    auto it = _values.find(unicode);

    if (it != _values.end())
    {
        return &it->second._value;
    }
    else
    {
        return &_values.find(invalid_replacement)->second._value;
    }
}
