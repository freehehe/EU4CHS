#pragma once
#include <string>

class VFS
{
public:
	static void EnumerateFolder(const std::string &folder);
	static void EnumerateOurFiles();
	static void Patch();
};
