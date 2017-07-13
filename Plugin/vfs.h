#pragma once

class VFS
{
public:
	static const char *MakeOurPath(const char *vfspath);
	static void EnumerateOurFiles();
	static void Patch();
};
