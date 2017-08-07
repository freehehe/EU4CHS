#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <map>

class PO2YML
{
private:
	struct TextMeta
	{
		std::string _name;
		std::string _paradox_flag;
		std::string _original;
		std::string _translated;
	};

	//Convert a file to our std::map
	void ReadPO(const std::string &in_fullpath);

	//Write all data in std::map to output folder
	void WriteYMLs(const std::string &out_folder) const;

	//Write all data in std::map to output folder\allin1.yml
	void WriteYML(const std::string &out_folder) const;

	static void ReplaceWeirdTokens(TextMeta &meta);
	static void ValidateText(const TextMeta &meta);

private:
	std::map<std::string, std::map<unsigned long, TextMeta>> _files;

public:
	void ConvertAll(const char *folder);
};
