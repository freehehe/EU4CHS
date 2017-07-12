#include "vfs.h"
#include <string>
#include <string_view>
#include <unordered_map>

static std::unordered_map<std::string_view, std::string> paths;

//From: \Europa Universalis IV\localisation\
//To: \Europa Universalis IV\scripts\eu4chs\localisation\

const char *VirtualFS::MakeOurPath(const char *vfspath)
{
	std::string result("scripts\\eu4chs\\");
	result += vfspath;


}

void VirtualFS::EnumerateOurFiles()
{

}
