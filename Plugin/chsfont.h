#pragma once
#include "stdinc.h"
#include "eu4.h"

class CChsFont
{
    std::vector<std::pair<uint8, uint8>> _table;

public:
    void ReadTable(const std::experimental::filesystem::path &filename);

    uint8 GetRow(uint32 cp) const;
    uint8 GetColumn(uint32 cp) const;
};
