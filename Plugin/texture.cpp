#include "stdinc.h"
#include "texture.h"
#include "eu4.h"

static std::size_t refcount;
static void *hTexture;

void CTexture::LoadTextureCallback()
{
	if (refcount == 0)
	{

	}

	++refcount;
}

void CTexture::UnloadTextureCallback()
{
	if (refcount == 1)
	{

	}

	--refcount;
}

void *CTexture::GetCHSTexture()
{
	return hTexture;
}

void CTexture::Patch()
{

}
