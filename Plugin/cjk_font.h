#pragma once
#include "stdinc.h"
#include "eu4.h"

class CJKFont
{
public:
    //字符信息，比eu4多了个贴图索引
    struct CharacterValues
    {
        EU4CharacterValues EU4Values;
        std::uint16_t PageIndex;
    };

    struct OurVertex
    {
        CVector4<float> xyzrhw;
        CVector2<float> uv;
        char pad[8];
    };
    VALIDATE_SIZE(OurVertex,32)

    //字库里没有的字会被替换
    static const std::uint32_t invalid_replacement = L'？';

    CJKFont();
    CJKFont(const std::experimental::filesystem::path &fntname);

    //加载fnt文件
    void InitWithFile(const std::experimental::filesystem::path &fntname);

    //加载贴图
    void LoadTexturesDX9();

    //释放贴图
    void UnloadTexturesDX9();

    //获取字符信息
    CharacterValues *GetValue(std::uint32_t unicode);

    //查询Kerning
    std::int16_t GetKerning(uint32_t first, uint32_t second) const;

    //获取字符所在的贴图
    TextureGFX *GetTexture(std::uint32_t unicode);

    //把字符绘制信息送入顶点缓存
    void SetPrimitivesDX9(std::uint32_t unicode, const CRect<int> *dstRect, std::uint32_t color);

    //渲染顶点缓存
    void DrawAllDX9();

private:
    //只读取一次fnt
    bool _initialized;

    //fnt所在的文件夹
    std::experimental::filesystem::path _workingdir;

    //贴图分辨率
    std::uint16_t _scaleW;
    std::uint16_t _scaleH;

    //贴图数
    std::uint16_t _pages;

    //字符信息
    std::unordered_map<std::uint32_t, CharacterValues> _values;

    //Kerning
    std::unordered_map<std::uint64_t, std::int16_t> _kernings;

    //贴图资源
    std::vector<TextureGFX> _textures;

    //贴图文件名
    std::vector<std::string> _texturenames;

    //顶点缓冲
    std::vector<std::vector<OurVertex>> _vertices;

    //读取二进制fnt文件
    //http://www.angelcode.com/products/bmfont/doc/file_format.html#bin
    void ReadInfoBlock(FILE *file);
    void ReadCommonBlock(FILE *file);
    void ReadPagesBlock(FILE *file);
    void ReadCharsBlock(FILE *file);
    void ReadKerningsBlock(FILE *file);
    void SetKernings();
};
