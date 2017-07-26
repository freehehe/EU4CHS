#include "stdinc.h"
#include "texture.h"
#include "eu4.h"

static std::map<std::size_t, void *> textures;
static void *hTexture;

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

		hTexture = nullptr;
	}
}

void *CTexture::GetCHSTexture()
{
	return hTexture;
}

void CTexture::Patch()
{

}
