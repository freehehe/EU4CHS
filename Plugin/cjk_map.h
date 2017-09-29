#pragma once
#include "cjk_font.h"

class CJKMapFont :public CJKFont
{
public:
    CJKMapFont(const std::experimental::filesystem::path &fntname);

private:
    std::vector<std::vector<SProvinceTextVertex>> _ProvinceBuffer;
};
