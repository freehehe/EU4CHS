#include "NonLatinFont.h"

using namespace std;
using namespace std::experimental;

NonLatinFont::NonLatinFont()
{
    _values.reserve(30000);
    _kernings.reserve(300);
}

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

        _values.emplace_back(unicode, values);
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

        _kernings.emplace_back(cp._packed, binary.value);
    }
}

void NonLatinFont::SetKernings()
{
    for (auto &kp : _kernings)
    {
        UnicodeCharPair cp;

        cp._packed = kp.first;

        find_if(_values.begin(),_values.end(),
            [cp](const pair<uint32, CharacterValues> &entry)
        {
            return entry.first == cp._first;
        })->second._value.kerning = true;
    }
}

void NonLatinFont::SortData()
{
    sort(_values.begin(), _values.end(), 
        [](pair<uint32_t, CharacterValues> &left, pair<uint32_t, CharacterValues> &right)
    {
        return left.first < right.first;
    });

    sort(_kernings.begin(), _kernings.end());
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
    SortData();
}

void NonLatinFont::LoadTexturesDX9()
{
    _textures.clear();

    for (auto &name : _texturenames)
    {
        TextureGFX gfx;

        //D3DXCreateTextureFromFileA(*(*game_meta.ppMasterContext)->GetDXDevice(), (_workingdir / name).string().c_str(), &gfx.field_0);
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

std::int16_t NonLatinFont::GetKerning(uint32_t first, uint32_t second)
{
    UnicodeCharPair duochar;

    duochar._first = first;
    duochar._second = second;

    auto it = lower_bound(_kernings.begin(), _kernings.end(), duochar._packed,
        [duochar](const pair<uint64_t, int16_t> &entry, uint64_t pack)
    {
        return entry.first == pack;
    });

    if (it != _kernings.end() && it->first == duochar._packed)
    {
        return it->second;
    }
    else
    {
        return 0;
    }
}

EU4CharacterValues * NonLatinFont::GetValue(uint32_t unicode)
{
    auto it = lower_bound(_values.begin(), _values.end(), unicode,
        [](const pair<uint32, CharacterValues> &entry, uint32_t code)
    {
        return entry.first == code;
    });

    if (it != _values.end() && it->first == unicode)
    {
        return &it->second._value;
    }
    else
    {
        return &lower_bound(_values.begin(), _values.end(), invalid_replacement,
            [](const pair<uint32, CharacterValues> &entry, uint32_t code)
        {
            return entry.first == code;
        })->second._value;
    }
}

TextureGFX * NonLatinFont::GetTexture(std::uint32_t unicode)
{
    auto it = lower_bound(_values.begin(), _values.end(), unicode,
        [](const pair<uint32, CharacterValues> &entry, uint32_t code)
    {
        return entry.first == code;
    });

    if (it != _values.end() && it->first == unicode)
    {
        return &_textures[it->second._page];
    }
    else
    {
        return &_textures[lower_bound(_values.begin(), _values.end(), invalid_replacement,
            [](const pair<uint32, CharacterValues> &entry, uint32_t code)
        {
            return entry.first == code;
        })->second._page];
    }
}
