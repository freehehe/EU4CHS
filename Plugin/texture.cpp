#include "stdinc.h"
#include "texture.h"
#include "eu4.h"
#include "plugin.h"

static LPDIRECT3DTEXTURE9 hFontTexture;
static LPDIRECT3DTEXTURE9 hMapFontTexture;
static LPDIRECT3D9 *ppObject;
static LPDIRECT3DDEVICE9 *ppDevice;
static std::size_t refCount;

static void *retAddress;

__declspec(naked) void GetDeviceHook()
{
	__asm
	{
		pop retAddress;
		mov ppObject, edi;
		lea eax, [edi + 4];
		mov ppDevice, eax;
		mov eax, game_meta.pMasterContext;
		mov [eax], edi;
		jmp retAddress;
	}
}

void LoadTextureHook()
{
	if (refCount == 0)
	{
		D3DXCreateTextureFromFileA(*ppDevice, Plugin::GetFontPath(), &hFontTexture);
		D3DXCreateTextureFromFileA(*ppDevice, Plugin::GetMapFontPath(), &hMapFontTexture);
	}

	++refCount;
}

void UnloadTextureHook()
{
	if (refCount == 1)
	{
		hFontTexture->Release();
		hMapFontTexture->Release();
	}

	--refCount;
}

void CTexture::Patch()
{
	
}
