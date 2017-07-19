#include "plugin.h"
#include "eu4.h"
#include <vector>
#include <iterator>
#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>
#include "../include/utf8cpp/utf8.h"
#include "../include/injector/calling.hpp"

std::vector<char> u8sequence;
std::vector<char> latin1s(0x10000);
std::vector<std::pair<std::uint32_t, char>> result(0xFFFF);

void UTF8ToLatin1View()
{
	for (std::uint32_t cp = 1; cp < 0x10000; ++cp)
	{
		utf8::unchecked::append(cp, back_inserter(u8sequence));
	}

	u8sequence.push_back(0);

	injector::fastcall<void(const char *, char *)>::call(game.pfConvertUTF8ToLatin1, u8sequence.data(), latin1s.data());

	latin1s.pop_back();

	for (std::size_t index = 0; index < latin1s.size(); ++index)
	{
		result[index].first = index + 1;
		result[index].second = latin1s[index];
	}

	result.erase(std::remove_if(result.begin(), result.end(), 
		[](std::pair<std::uint32_t, char> &p)
	{
		return (p.first > 255) && (p.second == '?');
	}), result.end());
}

BOOL WINAPI DllMain(HMODULE module, DWORD reason, void *reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Plugin::Init(module);

		//UTF8ToLatin1View();
	}

	return TRUE;
}
