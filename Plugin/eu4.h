#pragma once
#include "stdinc.h"

#define VALIDATE_SIZE(type,size) static_assert(sizeof(type)==size);

struct IncompleteClass
{
    template <typename T, std::uintptr_t offset>
    T *field()
    {
        return (T *)(reinterpret_cast<std::uintptr_t>(this) + offset);
    }
};

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

class CString
{
    union
    {
        char *heap;
        char sso_head[4];
    };

    char sso_rest[12];
    std::size_t length;
    std::size_t capacity;

public:
    CString() = delete;

    const char *c_str() const
    {
        return capacity > 15 ? heap : sso_head;
    }
};
VALIDATE_SIZE(CString, 0x18)

template <typename T>
struct CArray
{
    T *_Myfirst;
    T *_Mylast;
    T *_Myend;
};

struct TextureGFX
{
    LPDIRECT3DTEXTURE9 field_0 = nullptr;
    bool field_4 = false;
    DWORD field_8 = 0;
    int field_C = 0;
    int field_10 = 0;
    int field_14 = 0;
};
VALIDATE_SIZE(TextureGFX, 0x18)

struct CToken
{
    int _Type; //0xF - text
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

struct EU4CharacterValues
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
VALIDATE_SIZE(EU4CharacterValues, 0x10)

struct CBitmapFontCharacterSet :IncompleteClass
{
    EU4CharacterValues *GetLatin1Value(uint32_t cp)
    {
        return field<EU4CharacterValues *, 0>()[cp];
    }

    float *GetScaleX()
    {
        return field<float, 0x428>();
    }
};

struct CBitmapFont :IncompleteClass
{
    CBitmapFontCharacterSet *GetLatin1CharacterSet()
    {
        return field<CBitmapFontCharacterSet, 0xB4>();
    }

    const CString *GetFontPath()
    {
        //e.g.  gfx/fonts/vic18
        return field<const CString, 0x9C>();
    }

    EU4CharacterValues *GetLatin1Value(uint32_t cp)
    {
        return GetLatin1CharacterSet()->GetLatin1Value(cp);
    }
};

struct EU4Meta
{
    std::uintptr_t pfCBitmapFontCharacterSet_GetKerning;

    FARPROC pfPHYSFS_openRead;

    std::uintptr_t pfGfxInitDX9;
    std::uintptr_t pfGfxShutdownDX9;
    std::uintptr_t pfGfxDrawDX9;

    LPDIRECT3DDEVICE9 pDX9Device;

    char *pOriginalText;
    char *pWord;
    char *pText;

    EU4Meta();
};

extern EU4Meta game_meta;
