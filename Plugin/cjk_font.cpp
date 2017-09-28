#include "cjk_font.h"
#include "functions.h"

using namespace std;
using namespace std::experimental;

CJKFontBase::CJKFontBase(const filesystem::path & fntname)
{
    InitWithFile(fntname);
    
    _Values.reserve(45000);
}

void CJKFontBase::ReadInfoBlock(FILE * file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);
    fseek(file, block_size, SEEK_CUR);
}

void CJKFontBase::ReadCommonBlock(FILE * file)
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

void CJKFontBase::ReadPagesBlock(FILE *file)
{
    uint32_t block_size;
    fread(&block_size, 4, 1, file);

    _PagesBlock.resize(block_size);

    fread(_PagesBlock.data(), block_size, 1, file);
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

        _Values[unicode] = values;
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

    _BaseBuffer.resize(_PageCount);

    _Textures.resize(_PageCount);
    char *pName = _PagesBlock.data();

    for (size_t index = 0; index < _PageCount; ++index)
    {
        _Textures[index].first = pName;
        pName += (strlen(pName) + 1);
    }
}

void CJKFontBase::LoadTexturesDX9()
{
    for (auto &info : _Textures)
    {
        D3DXCreateTextureFromFileW(g_game.pDX9Device, (_WorkingDir / info.first).replace_extension(".dds").c_str(), &info.second);
    }
}

void CJKFontBase::UnloadTexturesDX9()
{
    for (auto &info : _Textures)
    {
        info.second->Release();
    }
}

const CJKFontBase::CJKCharInfo &CJKFontBase::GetValue(uint32_t unicode)
{
    auto it = _Values.find(unicode);

    if (it != _Values.end())
    {
        return it->second;
    }
    else
    {
        return _Values[INVALID_REPLACEMENT];
    }
}

const EU4CharInfo * CJKFontBase::GetEU4Value(uint32_t unicode)
{
    return &GetValue(unicode).Value;
}
