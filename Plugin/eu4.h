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
	char gap_C[4];
};

struct EU4Meta
{
	std::uintptr_t pfCTextureHandler_AddTexture;
	std::uintptr_t pfCTextureHandler_GetTexture;
	std::uintptr_t pfCTextureHandler_RemoveTexture;

	std::uintptr_t pfLoadDDSTextureFromMemory;

	std::uintptr_t pfCBitmapFontCharacterSet_GetKerning;
	std::uintptr_t pfConvertUTF8ToLatin1;
	std::uintptr_t pfCBitmapFont_GetWidthOfString;
	std::uintptr_t pfCBitmapFont_GetHeightOfString;
	std::uintptr_t pfCBitmapFont_RenderToScreen;
	std::uintptr_t pfCBitmapFont_RenderToTexture;

	std::uintptr_t pfCTextBuffer_GetCharacterAt;

	std::uintptr_t *pfGFXDraw;
	std::uintptr_t *pfGFXSetTexture;

	std::uintptr_t pfVFSOpenFile;

	void *pfPHYSFS_openRead;

	char *pOriginalText;
	char *pWord;
	char *pProcessedText;

	EU4Meta();
};

extern EU4Meta game_meta;
