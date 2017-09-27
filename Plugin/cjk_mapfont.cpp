#include "cjk_mapfont.h"
#include "functions.h"

using namespace std;
using namespace std::experimental;

CJKMapFont::CJKMapFont(const filesystem::path & fntname)
    :CJKFont(fntname)
{
}

void CJKMapFont::AddVerticesDX9(CBitmapFont *pFont, std::uint32_t unicode, SProvinceTextVertex * pVertices)
{
    float width_ratio = (float)pFont->get_field<int, 0x4E8>() / _TextureWidth;
    float height_ratio = (float)pFont->get_field<int, 0x4EC>() / _TextureHeight;

    pVertices[0].UV.x *= width_ratio;
    pVertices[0].UV.y *= height_ratio;
    pVertices[1].UV.x *= width_ratio;
    pVertices[1].UV.y *= height_ratio;
    pVertices[2].UV.x *= width_ratio;
    pVertices[2].UV.y *= height_ratio;
    pVertices[3].UV.x *= width_ratio;
    pVertices[3].UV.y *= height_ratio;
    pVertices[4].UV.x *= width_ratio;
    pVertices[4].UV.y *= height_ratio;
    pVertices[5].UV.x *= width_ratio;
    pVertices[5].UV.y *= height_ratio;

    copy_n(pVertices, 6, back_inserter(_Vertices[GetValue(unicode)->TextureIndex]));
}

void CJKMapFont::DrawAllDX9()
{
    for (size_t index = 0; index < _PageCount; ++index)
    {
        if (!_Vertices[index].empty())
        {
            g_game.pDX9Device->SetTexture(0, _Textures[index]);

            g_game.pDX9Device->DrawIndexedPrimitiveUP(
                D3DPT_TRIANGLELIST,
                0, 
                _Vertices[index].size(),
                _Vertices[index].size() / 3, 
                _indices[index].data(), 
                D3DFORMAT::D3DFMT_INDEX32,
                _Vertices[index].data(),
                sizeof(SProvinceTextVertex));
        }
    }
}
