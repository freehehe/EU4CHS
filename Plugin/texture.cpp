#include "stdinc.h"
#include "texture.h"

void *hTexture;

void CTexture::LoadTextureCallback()
{
	if (!hTexture)
	{
	}
}

void CTexture::UnloadTextureCallback()
{
	if (hTexture)
	{
	}
}

void *CTexture::GetCHSTexture()
{
	return hTexture;
}

void CTexture::Patch()
{

}
