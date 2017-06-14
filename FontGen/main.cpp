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

void work(const char *folder)
{
	std::set<wchar_t> collection;
	std::vector<char> buffer;
	std::vector<wchar_t> wbuffer;
	std::ifstream ifs;

	char fullpath[512];

	WIN32_FIND_DATAA fda;

	std::strcpy(fullpath, folder);
	PathAppendA(fullpath, "*.yml");

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

		utf8::utf8to16(buffer.begin(), buffer.end(), std::back_inserter(wbuffer));

		for (auto chr : wbuffer)
		{
			if (chr > 255)
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
	work("C:\\SS");

	return 0;
}
