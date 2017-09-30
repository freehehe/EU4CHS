#pragma once
#include "stdinc.h"

template <typename T>
struct CPoint
{
    T x;
    T y;
};

template <typename T>
struct CRect
{
    CPoint<T> _Origin;
    CPoint<T> _Extension;
};

template <typename T>
struct CVector2
{
    T x;
    T y;
};

template <typename T>
struct CVector3
{
    T x;
    T y;
    T z;
};

template <typename T>
struct CVector4
{
    T x;
    T y;
    T z;
    T w;
};

struct CCursorPosition
{
    uint16_t row;
    uint16_t column;
};

struct CToken
{
    int _LexerToken;
    char _szVal[512];
    bool _bExplicitString;
};
VALIDATE_SIZE(CToken,520)

struct STextVertex
{
    CVector3<float> Position;
    CVector2<float> UV;
    uint32_t FillColor;
    uint32_t BorderColor;
};
VALIDATE_SIZE(STextVertex,0x1C)

struct SProvinceTextVertex
{
    CVector3<float> Position;
    CVector2<float> UV;
};
VALIDATE_SIZE(SProvinceTextVertex, 0x14)

struct CInputEvent
{
    char _data[0x58];

    void Init(char character)
    {
        memset(this, 0, sizeof(CInputEvent));

        _data[0xC] = character;
        _data[0x38] = 3;
        _data[0x50] = 2;
    }
};
VALIDATE_SIZE(CInputEvent,0x58)

struct EU4Meta
{
    LPDIRECT3DDEVICE9 pDX9Device;

    std::uintptr_t pfGfxInitDX9;
    std::uintptr_t pfGfxShutdownDX9;
    std::uintptr_t pfGfxDrawDX9;

    std::uintptr_t pfCBitmapCharacterSet_GetKerning;

    std::uintptr_t pfCTextureHandler_AddTexture;
    std::uintptr_t pfCTextureHandler_RemoveTextureInternal;

    STextVertex *pBitmapVertices;

    std::uintptr_t pfCString_Assign;

    char *pOriginalText;
    char *pWord;
    char *pText;
 
    EU4Meta();
};

extern EU4Meta g_game;
