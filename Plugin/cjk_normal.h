#pragma once
#include "cjk_font.h"

class CJKNormalFont :public CJKFontBase
{
public:
    CJKNormalFont(const std::experimental::filesystem::path &fntname);

    void AddVerticesDX9(CBitmapFont *pFont, uint32_t unicode, STextVertex *pVertex);

    //未缓存的绘制
    //直接绘制已有缓存
    //首次绘制并缓存
    void DrawUnbufferedDX9();
    void DrawBufferedDX9(uint32_t hash);
    void DrawAndBufferDX9(uint32_t hash);

private:
    std::vector<std::vector<STextVertex>> _BaseBuffer;
    std::map<uint32_t, std::vector<std::vector<STextVertex>>> _ScreenBuffer;
};
