#include "plugin.h"
#include "eu4.h"

void UTF8ToLatin1View()
{
    std::vector<char> u8sequence; 
    std::vector<std::uint32_t> u32sequence;
    std::vector<unsigned char> latin1s(0x10000);
    std::vector<std::pair<std::uint32_t, char>> result;

    for (std::uint32_t cp = 1; cp < 0x10000; ++cp)
    {
        u32sequence.push_back(cp);
    }

    utf8::utf16to8(u32sequence.begin(), u32sequence.end(), std::back_inserter(u8sequence));

    u8sequence.push_back(0);

    injector::fastcall<void(const char *, unsigned char *)>::call(g_pattern.find_first("81 EC B0 00 00 00 53 56 8B F1 8B DA").integer(-0x18), u8sequence.data(), latin1s.data());

    latin1s.pop_back();

    for (std::uint32_t index = 0; index < latin1s.size(); ++index)
    {
        std::uint32_t cp = u32sequence[index];
        unsigned char latin1 = latin1s[index];

        if (cp != latin1 && (cp < 256 || latin1 != '?'))
        {
            result.emplace_back(cp, latin1);
        }
    }

    std::cout << result.size();
}

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_plugin.InitAndPatch(module);
    }

    return TRUE;
}
