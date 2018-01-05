#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include "../include/utf8cpp/utf8.h"

using std::experimental::filesystem::v1::path;
using std::experimental::filesystem::v1::directory_iterator;

static void CollectChars(const path &filename, std::set<uint32_t> &collection)
{
	std::ifstream stream{ filename };

	if (!stream)
	{
		return;
	}

	std::istreambuf_iterator<char> stream_it{ stream };

	std::vector<uint32_t> wide_text;

	utf8::utf8to32(stream_it, std::istreambuf_iterator<char>{}, std::back_inserter(wide_text));

	for (auto wide_char : wide_text)
	{
		collection.insert(wide_char);
	}
}

static std::set<uint32_t> ScanFolder(const path &folder)
{
	std::set<uint32_t> result;

	directory_iterator dir_it{ folder };

	while (dir_it != directory_iterator{})
	{
		path filename = dir_it->path();

		if (filename.extension() == ".txt" || filename.extension() == ".yml")
		{
			CollectChars(filename, result);
		}

		++dir_it;
	}

	result.erase(0xFEFF);

	return result;
}

static void GenerateTable(const std::set<uint32_t> &collection, const path &text)
{
	std::vector<char> buffer;

	utf8::append(0xFEFF, std::back_inserter(buffer));
	utf8::utf32to8(collection.begin(), collection.end(), std::back_inserter(buffer));

	std::ofstream ofs(text, std::ios::trunc);

	if (!ofs)
	{
		return;
	}

	std::copy(buffer.begin(), buffer.end(), std::ostreambuf_iterator<char>(ofs));

	ofs.close();
}

//.exe 文件夹 输出文本
int main(int argc, char **argv)
{
	if (argc != 3) 
	{
		return 0;
	}

	std::set<uint32_t> collection = ScanFolder(argv[1]);
	GenerateTable(collection, argv[2]);
}
