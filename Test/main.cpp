#include <windows.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <map>

//pair[vfspath, ourvfspath]
//C:/games/eu4/
//C:/games/eu4/scripts/eu4chs/
//C:/games/eu4/scripts/eu4chs.asi

static std::map<std::size_t, std::string> files;
static std::string ourroot;
static std::string gameroot;

//From: localisation\xxx.xxx
//To: scripts\eu4chs\localisation\xxx.xxx
//Additional: scripts\eu4chs\

const char *MakeOurPath(const char *vfspath)
{
	auto it = files.find(std::hash<std::string_view>()(vfspath));

	return (it == files.end()) ? vfspath : it->second.c_str();
}

void EnumerateFolder(const std::string &folder)
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

void EnumerateOurFiles()
{
	char buffer[512];

	files.clear();

	HMODULE hself = GetModuleHandleA(NULL);

	GetModuleFileNameA(NULL, buffer, 512);

	*(std::strrchr(buffer, '\\')) = 0;

	gameroot = buffer;

	GetModuleFileNameA(hself, buffer, 512);

	*std::strrchr(buffer, '.') = 0;

	ourroot = buffer;

	std::replace(gameroot.begin(), gameroot.end(), '\\', '/');
	std::replace(ourroot.begin(), ourroot.end(), '\\', '/');

	EnumerateFolder(ourroot);
}

int main()
{
	EnumerateOurFiles();

	std::cout << MakeOurPath("include/QtCore/QVector") << std::endl;
	std::cout << MakeOurPath("include/qtcore/QVector") << std::endl;
	return 0;
}
