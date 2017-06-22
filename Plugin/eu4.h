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

	char *data()
	{
		return capacity > 15 ? heap : sso_head;
	}

	const char *data() const
	{
		return capacity > 15 ? heap : sso_head;
	}
};
VALIDATE_SIZE(CString, 0x18)

void *eu4_malloc(std::size_t size);
void eu4_free(void *p, int a2, int a3);
