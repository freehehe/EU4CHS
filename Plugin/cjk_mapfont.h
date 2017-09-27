#pragma once
#include "stdinc.h"
#include "eu4.h"
#include "cjk_font.h"

class CJKMapFont :public CJKFont
{
public:
    CJKMapFont(const std::experimental::filesystem::path &fntname);

    void AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, SProvinceTextVertex *pVertices);
    void DrawAllDX9();

protected:
    std::vector<std::vector<SProvinceTextVertex>> _Vertices;
    std::vector<std::vector<UINT>> _indices;
};
