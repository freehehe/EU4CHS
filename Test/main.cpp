#include <windows.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <iostream>

//pair[vfspath, fullpath]
static std::unordered_map<std::string_view, std::string> files;
static std::string ourroot; //Should be: C:/gameroot/scripts/test/
static std::string gameroot; //Should be: C:/gameroot/

//From: localisation\xxx.xxx
//To: scripts\eu4chs\localisation\xxx.xxx

const char *MakeOurPath(const char *vfspath)
{
	auto it = files.find(vfspath);

	return (it == files.end()) ? vfspath : it->second.c_str();
}

void EnumerateFolder(const char *folder)
{
	WIN32_FIND_DATAA fda;

	std::string virtualpath;
	std::string subfolder;
	std::string filename(folder);

	filename += "\\*";

	HANDLE hFind = FindFirstFileA(filename.c_str(), &fda);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		if (fda.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//文件夹
			if (fda.cFileName[0] == '.')
			{
				continue;
			}

			subfolder = folder;
			subfolder += '\\';
			subfolder += fda.cFileName;

			EnumerateFolder(subfolder.c_str());
		}
		else
		{
			//文件
			filename = folder;
			filename += '\\';
			filename += fda.cFileName;
			files.emplace(std::string_view(filename.data() + gameroot.length()), filename);
		}
	} while (FindNextFileA(hFind, &fda));
}

void EnumerateOurFiles()
{
	char buffer[512];

	files.clear();

	HMODULE hself = GetModuleHandleA(NULL);

	GetModuleFileNameA(NULL, buffer, 512);

	*(std::strrchr(buffer, '\\') + 1) = 0;

	gameroot = buffer;

	GetModuleFileNameA(hself, buffer, 512);

	*std::strrchr(buffer, '.') = 0;

	ourroot = buffer;
	
	EnumerateFolder(ourroot.c_str());
}

int main()
{
	EnumerateOurFiles();

	std::cout << MakeOurPath("include\\QtCore\\QVector") << std::endl;
	std::cout << MakeOurPath("include\\qtcore\\QVector") << std::endl;
	return 0;
}
