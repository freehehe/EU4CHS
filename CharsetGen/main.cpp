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

//.exe 文件夹 输出文本
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		return 0;
	}

	auto collection = ScanFolder(argv[1]);


}
