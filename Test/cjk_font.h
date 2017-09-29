#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <filesystem>

struct EU4CharInfo
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
    int16_t xoff;
    int16_t yoff;
    int16_t xadvance;
    bool kerning;
};

class CJKFontBase
{
public:
    struct CJKCharInfo
    {
        EU4CharInfo Value;
        uint16_t Page;
    };

    static const uint32_t INVALID_REPLACEMENT = L'？';

    CJKCharInfo *GetValueHash(uint32_t unicode);
    CJKCharInfo *GetValueMap(uint32_t unicode);
    CJKCharInfo *GetValueVec(uint32_t unicode);

    CJKFontBase(const std::experimental::filesystem::path &fntname);
    virtual ~CJKFontBase() = default;

private:
    void InitWithFile(const std::experimental::filesystem::path &fntname);

    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);

    std::unordered_map<uint32_t, CJKCharInfo> _HashValues;
    std::map<uint32_t, CJKCharInfo> _MapValues;
    std::vector<std::pair<uint32_t, CJKCharInfo>> _VecValues;
};
