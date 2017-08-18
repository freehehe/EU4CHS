#pragma once
#include "stdinc.h"

class CVFSManager
{
    std::unordered_map<std::size_t, std::string> files;

public:
	void EnumerateOurFiles();
	void Patch();
};
