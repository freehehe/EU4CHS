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
	std::uintptr_t pfCTextureHandler_LoadTexture;
	std::uintptr_t pfCTextureHandler_GetTexture;
	std::uintptr_t pfCTextureHandler_UnloadTexture;

	std::uintptr_t pfCBitMapFont_GetKerning;
	std::uintptr_t pfConvertUTF8ToLatin1;
	std::uintptr_t pfCBitMapFont_GetWidthOfString;
	std::uintptr_t pfCBitMapFont_GetHeightOfString;
	std::uintptr_t pfCBitMapFont_RenderToScreen;
	std::uintptr_t pfCBitMapFont_RenderToTexture;

	char *poriginal_text;
	char *pword;
	char *pprocessed_text;

	std::uintptr_t pfGFXSetTextureCallback;

	EU4Game();
};

extern EU4Game game;
