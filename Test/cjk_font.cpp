#include "cjk_font.h"
#include <algorithm>

using namespace std;
using namespace std::experimental;

CJKFontBase::CJKCharInfo * CJKFontBase::GetValueHash(uint32_t unicode)
{
    auto it = _HashValues.find(unicode);

    if (it != _HashValues.end())
    {
        return &it->second;
    }
    else
    {
        return &_HashValues[L'？'];
    }
}

CJKFontBase::CJKCharInfo * CJKFontBase::GetValueMap(uint32_t unicode)
{
    auto it = _MapValues.find(unicode);

    if (it != _MapValues.end())
    {
        return &it->second;
    }
    else
    {
        return &_MapValues[L'？'];
    }
}

CJKFontBase::CJKCharInfo * CJKFontBase::GetValueVec(uint32_t unicode)
{
    auto it = std::lower_bound(_VecValues.begin(), _VecValues.end(), unicode,
        [](std::pair<uint32_t, CJKCharInfo> &p, uint32_t code)
    {
        return p.first < code;
    });

    if (it->first == unicode)
    {
        return &it->second;
    }
    else
    {
        return &std::lower_bound(_VecValues.begin(), _VecValues.end(), L'？',
            [](std::pair<uint32_t, CJKCharInfo> &p, uint32_t code)
        {
            return p.first < code;
        })->second;
    }
}

CJKFontBase::CJKFontBase(const filesystem::path & fntname)
{
    InitWithFile(fntname);

    _HashValues.reserve(45000);
    _VecValues.reserve(45000);
}

void CJKFontBase::ReadInfoBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFontBase::ReadCommonBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFontBase::ReadPagesBlock(FILE *file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFontBase::ReadCharsBlock(FILE * file)
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
        CJKCharInfo values;

        values.Page = binary.page;
        values.Value.x = binary.x;
        values.Value.y = binary.y;
        values.Value.w = binary.width;
        values.Value.h = binary.height;
        values.Value.xoff = binary.xoff;
        values.Value.yoff = binary.yoff;
        values.Value.xadvance = binary.xadvance;
        values.Value.kerning = false;

        uint32_t unicode = binary.code;

        _HashValues[unicode] = values;
        _MapValues[unicode] = values;
    }

    for (auto &p : _MapValues)
    {
        _VecValues.emplace_back(p);
    }
}

void CJKFontBase::ReadKerningsBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFontBase::InitWithFile(const filesystem::path &fntname)
{
    unsigned char block_type;

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
}
