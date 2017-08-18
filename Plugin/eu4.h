#pragma once
#include "stdinc.h"

#define VALIDATE_SIZE(type,size) static_assert(sizeof(type)==size);

typedef std::uint8_t uint8;
typedef std::uint16_t uint16;
typedef std::uint32_t uint32;
typedef std::uint64_t uint64;
typedef std::int8_t int8;
typedef std::int16_t int16;
typedef std::int32_t int32;
typedef std::int64_t int64;

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
    uint16 row;
    uint16 column;
};

struct CString
{
    union
    {
        char *heap;
        char sso_head[4];
    };

    char sso_rest[12];
    unsigned int length;
    unsigned int capacity;

    const char *data() const
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

struct SMasterContextDX9 : public IncompleteClass
{
    LPDIRECT3D9 *GetDXObject()
    {
        return field<LPDIRECT3D9, 0>();
    }

    LPDIRECT3DDEVICE9 *GetDXDevice()
    {
        return field<LPDIRECT3DDEVICE9, 4>();
    }
};

struct EU4Meta
{
    std::uintptr_t pfConvertUTF8ToLatin1;

    std::uintptr_t pfCBitmapFontCharacterSet_GetKerning;
    std::uintptr_t pfCBitmapFont_GetWidthOfString;
    std::uintptr_t pfCBitmapFont_GetHeightOfString;
    std::uintptr_t pfCBitmapFont_RenderToScreen;
    std::uintptr_t pfCBitmapFont_RenderToTexture;

    std::uintptr_t pfCTextBuffer_GetCharacterAt;

    SMasterContextDX9 **ppMasterContext;

    std::uintptr_t *pfGFXDraw;
    std::uintptr_t *pfGFXSetTextures;

    std::uintptr_t pfVFSOpenFile;
    void *pfPHYSFS_openRead;

    char *pOriginalText;
    char *pWord;
    char *pProcessedText;

    EU4Meta();
};

extern EU4Meta game_meta;
