#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <iterator>
#include "../include/utf8cpp/utf8.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <utility>
#include <vector>

void enumerate_chars(const char *folder)
{
	std::set<wchar_t> collection;
	std::vector<char> buffer;
	std::vector<wchar_t> wbuffer;
	std::ifstream ifs;

	char fullpath[512];

	WIN32_FIND_DATAA fda;

	std::strcpy(fullpath, folder);
	PathAppendA(fullpath, "*.*");

	HANDLE hfind = FindFirstFileA(fullpath, &fda);

	if (hfind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (fda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}

		std::strcpy(fullpath, folder);
		PathAppendA(fullpath, fda.cFileName);

		ifs.open(fullpath);

		if (!ifs)
		{
			continue;
		}

		buffer.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());

		wbuffer.clear();

		if (!utf8::starts_with_bom(buffer.begin(), buffer.end()))
		{
			continue;
		}

		utf8::utf8to16(buffer.begin(), buffer.end(), std::back_inserter(wbuffer));

		for (auto chr : wbuffer)
		{
			if (chr > 255 && chr != 0x2013 && chr != 0x2014 && chr != 0x2018 && chr != 0x2026)
			{
				collection.insert(chr);
			}
		}

		ifs.close();

	} while (FindNextFileA(hfind, &fda));

	FindClose(hfind);

	collection.erase(0xFEFF);
}

int main(int argc, char **argv)
{
	enumerate_chars(R"(C:\eu4chs_release\ymls)");

	return 0;
}
