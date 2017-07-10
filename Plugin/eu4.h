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

class EU4Game
{
public:
	std::uintptr_t pfCTextureHandler_AddTexture;
	std::uintptr_t pfCTextureHandler_GetTexture;

	std::uintptr_t pfCBitmapFont_GetKerning;
	std::uintptr_t pfConvertUTF8ToLatin1;
	std::uintptr_t pfCBitmapFont_GetWidthOfString;
	std::uintptr_t pfCBitmapFont_GetHeightOfString;
	std::uintptr_t pfCBitmapFont_RenderToScreen;
	std::uintptr_t pfCBitmapFont_RenderToTexture;

	std::uintptr_t *pfGFXDraw;
	std::uintptr_t *pfGFXSetTexture;

	char *poriginal_text;
	char *pword;
	char *pprocessed_text;

	EU4Game();
};

extern EU4Game game;
