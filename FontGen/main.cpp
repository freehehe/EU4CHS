#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <cstring>
#include <cstdio>
#include <iterator>
#include <iostream>
#include "../include/utf8cpp/utf8.h"

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

static set<uint32_t> collection;
static vector<uint32_t> wbuffer;
static vector<vector<uint32_t>> char_matrix(1);

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

		wbuffer.clear();

		utf8::utf8to32(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>(), back_inserter(wbuffer));
		
		for (auto chr : wbuffer)
		{
			if (chr > 255 &&
				chr != 0x152 &&
				chr != 0x153 &&
				chr != 0x160 &&
				chr != 0x161 &&
				chr != 0x178 &&
				chr != 0x17d &&
				chr != 0x17e &&
				chr != 0x192 &&
				chr != 0x2c6 &&
				chr != 0x2dc &&
				chr != 0x2013 &&
				chr != 0x2014 &&
				chr != 0x2018 &&
				chr != 0x2019 &&
				chr != 0x201a &&
				chr != 0x201c &&
				chr != 0x201d &&
				chr != 0x201e &&
				chr != 0x2020 &&
				chr != 0x2021 &&
				chr != 0x2022 &&
				chr != 0x2026 &&
				chr != 0x2030 &&
				chr != 0x2039 &&
				chr != 0x203a &&
				chr != 0x20ac &&
				chr != 0x2122)
			{
				collection.insert(chr);
			}
		}

		ifs.close();

	} while (FindNextFileA(hfind, &fda));

	FindClose(hfind);

	collection.erase(0xFEFF);
}

void generate_table()
{
	for (auto chr : collection)
	{
		if (char_matrix.back().size() == 128)
		{
			char_matrix.emplace_back();
		}

		char_matrix.back().emplace_back(chr);
	}

	wbuffer.clear();

	wbuffer.push_back(0xFEFF);

	for (auto &line : char_matrix)
	{
		copy(line.begin(), line.end(), back_inserter(wbuffer));
		wbuffer.push_back('\n');
	}

	ofstream ofs("C:/eu4chs_release/font/eu4chs.txt", ios::trunc);

	if (!ofs)
	{
		return;
	}

	utf8::utf32to8(wbuffer.begin(), wbuffer.end(), ostreambuf_iterator<char>(ofs));

	ofs.close();
}

int main(int argc, char **argv)
{
	enumerate_chars("C:/eu4chs_release/ymls");
	generate_table();

	return 0;
}
