#include <windows.h>
#include <KnownFolders.h>
#include <filesystem>
#include <ShlObj.h>
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")

using namespace std;
using namespace std::experimental;

struct
{
    HMODULE dll;
    FARPROC D3DPERF_BeginEvent;
    FARPROC D3DPERF_EndEvent;
    FARPROC D3DPERF_GetStatus;
    FARPROC D3DPERF_QueryRepeatFrame;
    FARPROC D3DPERF_SetMarker;
    FARPROC D3DPERF_SetOptions;
    FARPROC D3DPERF_SetRegion;
    FARPROC DebugSetLevel;
    FARPROC DebugSetMute;
    FARPROC Direct3D9EnableMaximizedWindowedModeShim;
    FARPROC Direct3DCreate9;
    FARPROC Direct3DCreate9Ex;
    FARPROC Direct3DShaderValidatorCreate9;
    FARPROC PSGPError;
    FARPROC PSGPSampleTexture;

    void LoadOriginalLibrary(HMODULE module)
    {
        dll = module;
        D3DPERF_BeginEvent = GetProcAddress(dll, "D3DPERF_BeginEvent");
        D3DPERF_EndEvent = GetProcAddress(dll, "D3DPERF_EndEvent");
        D3DPERF_GetStatus = GetProcAddress(dll, "D3DPERF_GetStatus");
        D3DPERF_QueryRepeatFrame = GetProcAddress(dll, "D3DPERF_QueryRepeatFrame");
        D3DPERF_SetMarker = GetProcAddress(dll, "D3DPERF_SetMarker");
        D3DPERF_SetOptions = GetProcAddress(dll, "D3DPERF_SetOptions");
        D3DPERF_SetRegion = GetProcAddress(dll, "D3DPERF_SetRegion");
        DebugSetLevel = GetProcAddress(dll, "DebugSetLevel");
        DebugSetMute = GetProcAddress(dll, "DebugSetMute");
        Direct3D9EnableMaximizedWindowedModeShim = GetProcAddress(dll, "Direct3D9EnableMaximizedWindowedModeShim");
        Direct3DCreate9 = GetProcAddress(dll, "Direct3DCreate9");
        Direct3DCreate9Ex = GetProcAddress(dll, "Direct3DCreate9Ex");
        Direct3DShaderValidatorCreate9 = GetProcAddress(dll, "Direct3DShaderValidatorCreate9");
        PSGPError = GetProcAddress(dll, "PSGPError");
        PSGPSampleTexture = GetProcAddress(dll, "PSGPSampleTexture");
    }
} d3d9meta;

void InitD3D9()
{
    wchar_t *pd3d9path;
    filesystem::path d3d9path;

    SHGetKnownFolderPath(FOLDERID_System, 0, NULL, &pd3d9path);
    d3d9path = filesystem::path{ pd3d9path } / L"d3d9.dll";
    CoTaskMemFree(pd3d9path);

    d3d9meta.LoadOriginalLibrary(LoadLibraryW(d3d9path.c_str()));
}

void LoadScripts(const filesystem::path &folder)
{
    filesystem::directory_iterator dirit{ folder };

    while (dirit != filesystem::directory_iterator{})
    {
        auto _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == L".dll")
        {
            LoadLibraryW(_path.c_str());
        }

        ++dirit;
    }
}

void Initialize(HMODULE hSelf)
{
    wchar_t pluginpath[MAX_PATH];

    GetModuleFileNameW(hSelf, pluginpath, MAX_PATH);

    InitD3D9();
    LoadScripts(filesystem::path{ pluginpath }.parent_path() / L"scripts");
}

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        Initialize(module);
    }

    return TRUE;
}

__declspec(naked) void _D3DPERF_BeginEvent() { _asm { jmp[d3d9meta.D3DPERF_BeginEvent] } }
__declspec(naked) void _D3DPERF_EndEvent() { _asm { jmp[d3d9meta.D3DPERF_EndEvent] } }
__declspec(naked) void _D3DPERF_GetStatus() { _asm { jmp[d3d9meta.D3DPERF_GetStatus] } }
__declspec(naked) void _D3DPERF_QueryRepeatFrame() { _asm { jmp[d3d9meta.D3DPERF_QueryRepeatFrame] } }
__declspec(naked) void _D3DPERF_SetMarker() { _asm { jmp[d3d9meta.D3DPERF_SetMarker] } }
__declspec(naked) void _D3DPERF_SetOptions() { _asm { jmp[d3d9meta.D3DPERF_SetOptions] } }
__declspec(naked) void _D3DPERF_SetRegion() { _asm { jmp[d3d9meta.D3DPERF_SetRegion] } }
__declspec(naked) void _DebugSetLevel() { _asm { jmp[d3d9meta.DebugSetLevel] } }
__declspec(naked) void _DebugSetMute() { _asm { jmp[d3d9meta.DebugSetMute] } }
__declspec(naked) void _Direct3D9EnableMaximizedWindowedModeShim() { _asm { jmp[d3d9meta.Direct3D9EnableMaximizedWindowedModeShim] } }
__declspec(naked) void _Direct3DCreate9() { _asm { jmp[d3d9meta.Direct3DCreate9] } }
__declspec(naked) void _Direct3DCreate9Ex() { _asm { jmp[d3d9meta.Direct3DCreate9Ex] } }
__declspec(naked) void _Direct3DShaderValidatorCreate9() { _asm { jmp[d3d9meta.Direct3DShaderValidatorCreate9] } }
__declspec(naked) void _PSGPError() { _asm { jmp[d3d9meta.PSGPError] } }
__declspec(naked) void _PSGPSampleTexture() { _asm { jmp[d3d9meta.PSGPSampleTexture] } }
