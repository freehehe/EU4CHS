#include "PO2YML.h"
#include <filesystem>
#include <fstream>
#include <regex>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <cstring>

//msgctxt "[序号][名称][P社特产flag]"
//msgid "原文"
//msgstr "译文"

using namespace std;
using namespace std::experimental;

void PO2YML::ConvertAll(const char *folder)
{
	string in_folder(folder);
	string out_folder(folder);

	filesystem::directory_iterator dirit(folder);

	_files.clear();

	while (dirit != filesystem::directory_iterator())
	{
		filesystem::directory_entry entry = *dirit;

		if (entry.path().extension() == ".po")
		{
			ReadPO(entry.path().string());
		}

		++dirit;
	}

	out_folder += "\\ymls\\";

	filesystem::create_directory(out_folder);


	WriteYML(out_folder);
}

void PO2YML::ReadPO(const string &in_fullpath)
{
	static regex fullpath_regex(R"([/\\]([^/\\]+)\.[0-9]+\.[Pp][Oo]$)");
	static regex msgctxt_regex(R"(msgctxt \"\[([0-9]+)\]\[([0-9A-Za-z_]+)\]\[([0-9]+)\]\")");
	static regex msgid_regex(R"(msgid \"(.*)\")");
	static regex msgstr_regex(R"(msgstr \"(.*)\")");
	static regex text_line_regex(R"(\"(.*)\")");

	string line;
	string *string_to_be_appended = nullptr;

	unsigned long index;
	TextMeta meta;

	smatch results;

	if (!regex_search(in_fullpath, results, fullpath_regex))
	{
		return;
	}
	
	auto filename_it = _files.emplace(results.str(1), map<unsigned long, TextMeta>()).first;

	ifstream ifs(in_fullpath);

	if (!ifs)
	{
		return;
	}

	ifs.seekg(3);

	while (getline(ifs, line))
	{
		if (line.empty())
		{
			//输出
			ReplaceWeirdTokens(meta);
			ValidateText(meta);

			if (!meta._name.empty())
			{
				auto entry_it = filename_it->second.find(index);

				if (entry_it == filename_it->second.end())
				{
					filename_it->second.emplace(index, meta);
				}
				else
				{
					entry_it->second = meta;
				}
			}

			meta._name.clear();
			meta._original.clear();
			meta._translated.clear();
			string_to_be_appended = nullptr;
			continue;
		}
		else if (line[0] == '#')
		{
			continue;
		}

		if (regex_match(line,results, msgctxt_regex))
		{
			index = stoul(results.str(1));
			meta._name = results.str(2);
			meta._paradox_flag = results.str(3);
			string_to_be_appended = &meta._name;
		}
		else if (regex_match(line, results, msgid_regex))
		{
			meta._original = results.str(1);
			string_to_be_appended = &meta._original;
		}
		else if (regex_match(line, results, msgstr_regex))
		{
			meta._translated = results.str(1);
			string_to_be_appended = &meta._translated;
		}
		else if (regex_match(line, results, text_line_regex))
		{
			if (string_to_be_appended)
			{
				string_to_be_appended->append(results.str(1));
			}
		}
	}
}

void PO2YML::WriteYMLs(const std::string &out_folder) const
{
	for (auto &file : _files)
	{
		ofstream ofs(out_folder + file.first + ".yml", ios::trunc);

		if (!ofs)
		{
			return;
		}

		ofs << "\xEF\xBB\xBFl_english:\n";

		for (auto &meta : file.second)
		{
			ofs << '#' << meta.second._name << ':' << meta.second._paradox_flag << ' ' << '"' << meta.second._original << '"' << '\n';
			ofs << ' ' << meta.second._name << ':' << meta.second._paradox_flag << ' ' << '"' << meta.second._translated << '"' << "\n\n";
		}
	}
}

void PO2YML::WriteYML(const std::string &out_folder) const
{
	ofstream ofs(out_folder + "allin1.yml", ios::trunc);

	if (!ofs)
	{
		return;
	}

	ofs << "\xEF\xBB\xBFl_english:\n";

	for (auto &file : _files)
	{
		for (auto &meta : file.second)
		{
			ofs << '#' << meta.second._name << ':' << meta.second._paradox_flag << ' ' << '"' << meta.second._original << '"' << '\n';
			ofs << ' ' << meta.second._name << ':' << meta.second._paradox_flag << ' ' << '"' << meta.second._translated << '"' << "\n\n";
		}
	}
}

void PO2YML::ReplaceWeirdTokens(TextMeta &meta)
{
	static regex token_A3_regex("<A3-([^>]+)>");
	static regex token_A4_regex("<A4>");
	static regex token_A7_regex("<A7-(.)>");
	static regex token_VAR_regex("<VAR-([^>]+)>");
	static regex newline_regex(R"(\\\\n)");
	static regex quote_regex(R"(\\\")");

	string temp(meta._translated);

	temp = regex_replace(temp, token_A3_regex, u8"£$1");
	temp = regex_replace(temp, token_A4_regex, u8"¤");
	temp = regex_replace(temp, token_A7_regex, u8"§$1");
	temp = regex_replace(temp, token_VAR_regex, "$$$1$$");
	temp = regex_replace(temp, newline_regex, R"(\n)");
	temp = regex_replace(temp, quote_regex, "\"");

	meta._translated = temp;
}

void PO2YML::ValidateText(const TextMeta &meta)
{
	auto bad_token_it = find(meta._translated.begin(), meta._translated.end(), '<');

	if (bad_token_it != meta._translated.end() && (bad_token_it[1] == 'A' || bad_token_it[1] == 'V'))
	{
		cout << "Bad token found in entry \"" << meta._name << "\":\n" << &*bad_token_it << '\n';
	}

	bad_token_it = find(meta._translated.begin(), meta._translated.end(), '\\');

	if (bad_token_it != meta._translated.end() && bad_token_it[1] != 'n')
	{
		cout << "Bad escape sequence found in entry \"" << meta._name << "\":\n" << &*bad_token_it << '\n';
	}
}
