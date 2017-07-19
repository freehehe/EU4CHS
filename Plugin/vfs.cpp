#include "plugin.h"
#include "vfs.h"
#include "eu4.h"
#include <windows.h>
#include <string>
#include <string_view>
#include <functional>
#include <map>
#include <algorithm>

#include "../include/injector/hooking.hpp"
#include "../include/injector/assembly.hpp"
#include "../include/injector/calling.hpp"

static std::map<std::size_t, std::string> files;
static std::string ourroot;
static std::string gameroot;

void VFS::EnumerateFolder(const std::string &folder)
{
	WIN32_FIND_DATAA fda;

	std::string vfspath;
	std::string ourvfspath;
	std::string subfolder;
	std::string filename;
	std::string search_name;

	search_name = folder;

	search_name += "/*";

	HANDLE hFind = FindFirstFileA(search_name.c_str(), &fda);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (fda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (fda.cFileName[0] == '.')
			{
				continue;
			}

			subfolder = folder;
			subfolder += '/';
			subfolder += fda.cFileName;

			EnumerateFolder(subfolder);
		}
		else
		{
			filename = folder;
			filename += '/';
			filename += fda.cFileName;
			vfspath = filename.data() + ourroot.length() + 1;
			ourvfspath = filename.data() + gameroot.length() + 1;

			files.emplace(std::hash<std::string_view>()(vfspath), ourvfspath);

		}
	} while (FindNextFileA(hFind, &fda));
}

void VFS::EnumerateOurFiles()
{
	char buffer[512];

	files.clear();

	GetModuleFileNameA(Plugin::GetEXEHandle(), buffer, 512);

	*(std::strrchr(buffer, '\\')) = 0;

	gameroot = buffer;

	GetModuleFileNameA(Plugin::GetASIHandle(), buffer, 512);

	*std::strrchr(buffer, '.') = 0;

	ourroot = buffer;
	ourroot += "\\vfsroot";

	std::replace(gameroot.begin(), gameroot.end(), '\\', '/');
	std::replace(ourroot.begin(), ourroot.end(), '\\', '/');

	EnumerateFolder(ourroot);
}

void *VFSOpenFile_0x8D(const char *vfspath)
{
	auto it = files.find(std::hash<std::string_view>()(vfspath));

	if (it != files.end())
	{
		vfspath = it->second.c_str();
	}

	return injector::cstd<void *(const char *)>::call(game.pfPHYSFS_openRead, vfspath);
};

void VFS::Patch()
{
	injector::MakeCALL(game.pfVFSOpenFile + 0x8D, VFSOpenFile_0x8D);
}
