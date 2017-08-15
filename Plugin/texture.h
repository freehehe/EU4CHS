#pragma once
#include "stdinc.h"
#include "eu4.h"

//vic_29s - shadow
//vic_22_bl - black
//vic_18_grey - grey
//tahoma_20_bold - bold

struct SLoadTextureSettings
{
	bool _bForcePOW2;
	bool _bIsRGB;
	bool _bCanBeLowRes;
	bool _bReportMissingTexture;
	int _nMipSkipCount;
	int _TagertFormatHint;
};
VALIDATE_SIZE(SLoadTextureSettings, 0xC)

struct SLoadTextureProperties
{
	int _nWidth;
	int _nHeight;
	int _nNumMipMaps;
	int _nFormat;
	int _Type;
	bool _bDynamic;
};
VALIDATE_SIZE(SLoadTextureProperties, 0x18)

struct STextureDataLocator
{
	int _Mip;
	int _CubeMapFace;
};
VALIDATE_SIZE(STextureDataLocator, 0x8)

struct STextureData
{
	STextureDataLocator _Locator;
	int _nWidth;
	int _nHeight;
	int _nDataSize;
	void *_pData;
};
VALIDATE_SIZE(STextureData, 0x18)
VALIDATE_SIZE(CArray<STextureData>, 0xC)

struct SLoadTextureData
{
	SLoadTextureProperties _Properties;
	CArray<STextureData> _TextureDatas;
	void *_pDataBuffer;
};
VALIDATE_SIZE(SLoadTextureData, 0x28)

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
VALIDATE_SIZE(STextureDescription, 0x1C)


class CTexture
{
public:
	static void Patch();
};
