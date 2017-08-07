#pragma once
#include "stdinc.h"
#include "eu4.h"

struct SLoadTextureProperties
{
	int _nWidth;
	int _nHeight;
	int _nNumMipMaps;
	int _nFormat;
	int _Type;
	bool _bDynamic;
};
VALIDATE_SIZE(SLoadTextureProperties,0x18)

struct STextureDataLocator
{
	int _Mip;
	int _CubeMapFace;
};
VALIDATE_SIZE(STextureDataLocator,0x8)

struct STextureData
{
	STextureDataLocator _Locator;
	int _nWidth;
	int _nHeight;
	int _nDataSize;
	void *_pData;
};
VALIDATE_SIZE(STextureData,0x18)
VALIDATE_SIZE(CArray<STextureData>,0x10)

struct SLoadTextureData
{
	SLoadTextureProperties _Properties;
	CArray<STextureData> _TextureDatas;
	void *_pDataBuffer;
};
VALIDATE_SIZE(SLoadTextureData,0x2C)

struct STextureDescription
{
	int _nWidth;
	int _nHeight;
	int _nFormat;
	int _nDepth;
	int _nPaddingWidth;
	int _nPaddingHeight;
	int _nNumMipMaps;
};
VALIDATE_SIZE(STextureDescription,0x1C)

class CTexture
{
public:
	static void *GetCHSTexture();
	static void LoadTextureCallback();
	static void UnloadTextureCallback();

	static void Patch();
};
