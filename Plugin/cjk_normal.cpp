#include "cjk_normal.h"

CJKNormalFont::CJKNormalFont(const std::experimental::filesystem::path &fntname)
    :CJKFontBase(fntname)
{

}

void CJKNormalFont::AddVerticesDX9(CBitmapFont *pFont, uint32_t unicode, STextVertex *pVertex)
{
    float fWidthRatio = (float)pFont->GetTextureWidth() / _TextureWidth;
    float fHeightRatio = (float)pFont->GetTextureHeight() / _TextureHeight;

    pVertex[0].UV.x *= fWidthRatio;
    pVertex[0].UV.y *= fHeightRatio;
    pVertex[1].UV.x *= fWidthRatio;
    pVertex[1].UV.y *= fHeightRatio;
    pVertex[2].UV.x *= fWidthRatio;
    pVertex[2].UV.y *= fHeightRatio;
    pVertex[3].UV.x *= fWidthRatio;
    pVertex[3].UV.y *= fHeightRatio;
    pVertex[4].UV.x *= fWidthRatio;
    pVertex[4].UV.y *= fHeightRatio;

    copy_n(pVertex, 6, back_inserter(_BaseBuffer[GetValue(unicode)->Page]));
}

void CJKNormalFont::DrawUnbufferedDX9()
{
    for (size_t index = 0; index < _BaseBuffer.size(); ++index)
    {
        g_game.pDX9Device->SetTexture(0, _Textures[index].second);
        g_game.pDX9Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, _BaseBuffer[index].size() / 3, _BaseBuffer[index].data(), sizeof(STextVertex));

        _BaseBuffer[index].clear();
    }
}

void CJKNormalFont::DrawBufferedDX9(uint32_t hash)
{
    auto it = _ScreenBuffer.find(hash);

    if (it != _ScreenBuffer.end())
    {
        for (size_t index = 0; index < it->second.size(); ++index)
        {
            g_game.pDX9Device->SetTexture(0, _Textures[index].second);
            g_game.pDX9Device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, it->second[index].size() / 3, it->second[index].data(), sizeof(STextVertex));
        }
    }
}

void CJKNormalFont::DrawAndBufferDX9(uint32_t hash)
{
    _ScreenBuffer[hash] = _BaseBuffer;
    DrawUnbufferedDX9();
}
