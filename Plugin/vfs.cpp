#include "vfs.h"
#include <windows.h>
#include <string>
#include <string_view>
#include <unordered_map>

static std::unordered_map<std::string_view, std::string> paths;

//From: localisation\xxx
//To: scripts\eu4chs\localisation\xxx

const char *VFS::MakeOurPath(const char *vfspath)
{
	std::string result("scripts\\eu4chs\\");
	result += vfspath;


}

void VFS::EnumerateOurFiles()
{
	HFIND
}
