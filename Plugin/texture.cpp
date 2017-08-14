#include "stdinc.h"
#include "texture.h"
#include "eu4.h"

static LPDIRECT3DTEXTURE9 hTexture;

void CTexture::LoadTextureCallback()
{

}

void CTexture::UnloadTextureCallback()
{
	hTexture->Release();
}

void *CTexture::()
{

}

void CTexture::Patch()
{

}
