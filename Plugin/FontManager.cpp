#include "stdinc.h"
#include "FontManager.h"
#include "plugin.h"
#include "byte_pattern.h"

using namespace std;
using namespace std::experimental;

void NonLatinFontManager::LoadFonts()
{
    filesystem::directory_iterator dirit{ CSingleton<CPlugin>::Instance().GetPluginDirectory() / "fonts" };

    while (dirit != filesystem::directory_iterator{})
    {
        filesystem::path _path = dirit->path();

        if (filesystem::is_regular_file(_path) && _path.extension() == ".fnt")
        {
            auto result = _fonts.emplace(hash<string_view>()(_path.filename().string().c_str()), NonLatinFont{});

            result.first->second.InitWithFile(_path);
        }

        ++dirit;
    }
}

void NonLatinFontManager::AfterDX9DeviceCreate()
{
    for (auto &font : CSingleton<NonLatinFontManager>::Instance()._fonts)
    {
        font.second.LoadTexturesDX9();
    }
}

void NonLatinFontManager::BeforeDX9DeviceRelease()
{
    for (auto &font : CSingleton<NonLatinFontManager>::Instance()._fonts)
    {
        font.second.UnloadTexturesDX9();
    }
}

NonLatinFont * NonLatinFontManager::GetFont(const CString * fontname)
{
    const char *cname = fontname->c_str();

    auto it = _fonts.find(hash<string_view>()(cname));

    if (it == _fonts.end())
    {
        return &it->second;
    }
    else
    {
        return &_fonts.find(hash<string_view>()("default"))->second;
    }
}

void NonLatinFontManager::InitAndPatch()
{

}
