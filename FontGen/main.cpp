#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <cstdio>
#include <iterator>
#include "../include/utf8cpp/utf8.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

static set<uint16_t> collection;
static vector<uint16_t> wbuffer;
static vector<vector<uint16_t>> char_matrix(1);
static vector<pair<uint8_t, uint8_t>> table(0x10000, pair<uint8_t, uint8_t>(63, 63));

void enumerate_chars(const char *folder)
{
	ifstream ifs;

	char fullpath[512];

	WIN32_FIND_DATAA fda;

	strcpy(fullpath, folder);
	PathAppendA(fullpath, "*.???");

	HANDLE hfind = FindFirstFileA(fullpath, &fda);

	if (hfind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		auto ext = find(begin(fda.cFileName), end(fda.cFileName), 0) - 4;

		if (_stricmp(ext, ".yml") != 0 && _stricmp(ext, ".txt") != 0)
		{
			continue;
		}

		if (fda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}

		strcpy(fullpath, folder);
		PathAppendA(fullpath, fda.cFileName);

		ifs.open(fullpath);

		if (!ifs)
		{
			continue;
		}

		istreambuf_iterator<char> itbeg(ifs);
		istreambuf_iterator<char> itend;

		if (!utf8::starts_with_bom(itbeg, itend))
		{
			continue;
		}

		wbuffer.clear();

		utf8::utf8to16(itbeg, itend, back_inserter(wbuffer));

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

void generate_data()
{
	for (auto chr : collection)
	{
		if (char_matrix.back().size() == 64)
		{
			char_matrix.emplace_back();
		}

		char_matrix.back().emplace_back(chr);

		table[chr].first = char_matrix.size() - 1;
		table[chr].second = char_matrix.back().size() - 1;
	}

	FILE *hf = fopen("C:\\eu4chs_release\\font\\eu4chs.txt", "wb");

	if (!hf)
	{
		return;
	}

	fwrite("\xFF\xFE", 2, 1, hf);

	for (auto &char_array : char_matrix)
	{
		fwrite(char_array.data(), 2, char_array.size(), hf);

		fwrite("\n\x00", 2, 1, hf);
	}

	fclose(hf);

	hf = fopen("C:\\eu4chs_release\\font\\eu4chs.dat", "wb");

	if (!hf)
	{
		return;
	}

	fwrite(table.data(), 2, 0x10000, hf);

	fclose(hf);
}

int main(int argc, char **argv)
{
	enumerate_chars(R"(C:\eu4chs_release\ymls)");
	generate_data();

	return 0;
}
