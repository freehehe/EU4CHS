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

    template <typename T, std::uintptr_t offset>
    T get_field()
    {
        return *(T *)(reinterpret_cast<std::uintptr_t>(this) + offset);
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
VALIDATE_SIZE(EU4CharInfo, 0x10)

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
    STextVertex *pVertices3d;

    std::uintptr_t pfCString_Assign;

    char *pOriginalText;
    char *pWord;
    char *pText;
 
    EU4Meta();
};

extern EU4Meta g_game;

class CString
{
    union
    {
        char *_heap;
        char _sso_head[4];
    };

    char _sso_rest[12];
    size_t _length;
    size_t _capacity;

public:
    CString() = delete;
    CString(const CString &) = delete;
    CString &operator=(const CString &) = delete;

    size_t length() const
    {
        return _length;
    }

    CString &assign(const char *cstr)
    {
        return injector::thiscall<CString &(CString *, const char *, size_t)>::call(g_game.pfCString_Assign, this, cstr, strlen(cstr));
    }

    const char *c_str() const
    {
        return _capacity > 15 ? _heap : _sso_head;
    }
};
VALIDATE_SIZE(CString, 0x18)

struct CBitmapCharacterSet :IncompleteClass
{
    const EU4CharInfo *GetLatin1Value(uint32_t cp)
    {
        if (cp == 0x3 || cp == 0x4 || cp == 0x7)
        {
            cp += 0xA0;
        }

        return field<EU4CharInfo *, 0>()[cp];
    }

    float GetScale()
    {
        return get_field<float, 0x428>();
    }
};

#define CHARSET_OFF 0xB4
struct CBitmapFont :IncompleteClass
{
    CBitmapCharacterSet *GetLatin1CharacterSet()
    {
        return field<CBitmapCharacterSet, CHARSET_OFF>();
    }

    const CString *GetFontPath()
    {
        return field<const CString, 0x9C>();
    }

    const EU4CharInfo *GetLatin1Value(uint32_t cp)
    {
        return GetLatin1CharacterSet()->GetLatin1Value(cp);
    }

    int GetTextureWidth()
    {
        return get_field<int, 0x4E8>();
    }

    int GetTextureHeight()
    {
        return get_field<int, 0x4EC>();
    }
};

extern struct HookContext
{
    CBitmapFont *pFont;
    CBitmapCharacterSet *pSet;

    CJKFontBase *cjkFont;

    uint32_t textLength;

    uint32_t unicode;
    ptrdiff_t unicodeLength;

    uint32_t nextUnicode;

    bool useSpecialChars;

    void *ret_addr;
} g_context;
