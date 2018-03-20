#pragma once
#include "stdinc.h"

namespace BitmapFont
{
    struct SBitmapCharacterValue
    {
        int16_t x;
        int16_t y;
        int16_t w;
        int16_t h;
        int16_t xoff;
        int16_t yoff;
        int16_t xadvance;
        bool kerning;
    };
    VALIDATE_SIZE(SBitmapCharacterValue, 0x10)

    typedef std::unordered_map<std::uint32_t, SBitmapCharacterValue> ValuesContainer;

    class CBitmapCharacterSet :IncompleteClass
    {
    public:
        ValuesContainer &GetContainer()
        {
            if (get_field<std::unordered_map<std::uint32_t, SBitmapCharacterValue>*, 0>() == nullptr)
            {
                *field<std::unordered_map<std::uint32_t, SBitmapCharacterValue> *, 0>() = new std::unordered_map<std::uint32_t, SBitmapCharacterValue>;
            }

            return *field<std::unordered_map<std::uint32_t, SBitmapCharacterValue>, 0>();
        }

        SBitmapCharacterValue *GetCharacterValue(std::uint32_t character)
        {
            return &GetContainer()[character];
        }
    };

    class CBitmapFont :IncompleteClass
    {
    public:
        CBitmapCharacterSet *GetCharacterSet()
        {
            return field<CBitmapCharacterSet, 0xB4>();
        }
    };

    void GetWidthOfStringPatch();
    void GetHeightOfStringPatch();
    void FillVertexBufferPatch();
    void RenderToScreenPatch();
    void RenderToTexturePatch();
    void GetRequiredSizePatch();
    void GetActualRequiredSizePatch();
    void GetActualRealRequiredSizeActuallyPatch();

    void InitAndPatch();
}
