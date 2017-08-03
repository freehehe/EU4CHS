#include "stdinc.h"
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

	utf8::unchecked::utf32to8(u32sequence.begin(), u32sequence.end(), std::back_inserter(u8sequence));

	u8sequence.push_back(0);

	injector::fastcall<void(const char *, unsigned char *)>::call(game.pfConvertUTF8ToLatin1, u8sequence.data(), latin1s.data());

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
		//Plugin::Init(module);

		UTF8ToLatin1View();
	}

	return TRUE;
}
