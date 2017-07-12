#pragma once

class VirtualFS
{
public:
	static const char *MakeOurPath(const char *vfspath);
	static void EnumerateOurFiles();
	static void Patch();
};
