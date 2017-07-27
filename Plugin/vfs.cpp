#include "stdinc.h"
#include "plugin.h"
#include "vfs.h"
#include "eu4.h"

static std::map<std::size_t, std::string> files;
static std::string ourroot;
static std::string gameroot;

void VFS::EnumerateFolder(const std::string &folder)
{
	std::string vfspath;
	std::string ourvfspath;

	std::experimental::filesystem::recursive_directory_iterator recur_it(folder);
	std::string_view root(folder);

	while (recur_it != std::experimental::filesystem::recursive_directory_iterator())
	{
		if (std::experimental::filesystem::is_regular_file(recur_it->path()))
		{
			const std::string &full_path = recur_it->path().string();

			vfspath = full_path.data() + ourroot.length() + 1;
			ourvfspath = full_path.data() + gameroot.length() + 1;

			std::replace(vfspath.begin(), vfspath.end(), '\\', '/');
			std::replace(ourvfspath.begin(), ourvfspath.end(), '\\', '/');

			files.emplace(std::hash<std::string_view>()(vfspath), ourvfspath);
		}

		++recur_it;
	}
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

	EnumerateFolder(ourroot);
}

static void *VFSOpenFile_0x8D(const char *vfspath)
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
