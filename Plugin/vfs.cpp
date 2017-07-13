#include "vfs.h"
#include "eu4.h"
#include <windows.h>
#include <string>
#include <string_view>
#include <unordered_map>

#include "../include/injector/hooking.hpp"
#include "../include/injector/assembly.hpp"
#include "../include/injector/calling.hpp"

//pair[vfspath, ourvfspath]
//ourvfspath=vfspath+"scripts\eu4chs\"

static std::unordered_map<std::string_view, std::string> file_list;
static std::string our_path_prefix; //scripts\eu4chs\

void *VFSOpenFile_0x8D(const char *vfspath)
{
	auto it = file_list.find(vfspath);

	if (it != file_list.end())
	{
		vfspath = it->second.c_str();
	}

	return injector::cstd<void *(const char *)>::call(game.pfPHYSFS_OpenRead, vfspath);
};

void VFS::Patch()
{
	injector::MakeCALL(game.pfVFSOpenFile + 0x8D, VFSOpenFile_0x8D);
}
