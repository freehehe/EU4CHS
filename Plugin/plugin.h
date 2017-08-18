#pragma once
#include "stdinc.h"

class CPlugin
{
    std::experimental::filesystem::path _font_path;
    std::experimental::filesystem::path _mapfont_path;
    std::experimental::filesystem::path _table_path;
    std::experimental::filesystem::path _vfs_dir;
    std::experimental::filesystem::path _game_dir;

    void Patch();

public:
    void Init(HMODULE hself);

    const std::experimental::filesystem::path &GetFontPath() const;
    const std::experimental::filesystem::path &GetMapFontPath() const;
    const std::experimental::filesystem::path &GetTablePath() const;
    const std::experimental::filesystem::path &GetVFSDirectory() const;
    const std::experimental::filesystem::path &GetGameDirectory() const;
};
