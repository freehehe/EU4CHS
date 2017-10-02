#pragma once
#include "stdinc.h"
#include "eu4.h"

class CString;
class CJKFont;

struct EU4CharInfo
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
VALIDATE_SIZE(EU4CharInfo, 0x10)

struct CBitmapCharacterSet :IncompleteClass
{
    EU4CharInfo *GetLatin1Value(uint32_t cp);
    CJKFont *GetCJKFont();
    float GetScaleX();
};

struct CBitmapFont :IncompleteClass
{
    CBitmapCharacterSet *GetLatin1CharacterSet();
    CJKFont *GetCJKFont();
    const CString *GetFontPath();
    EU4CharInfo *GetCharacterValue(uint32_t cp);
    int GetTextureWidth();
    int GetTextureHeight();

    static int __fastcall GetWidthOfString(CBitmapFont * pFont, int, const char * Text, const int nLength, bool bUseSpecialChars);
    static int __fastcall GetHeightOfString(CBitmapFont * pFont, int, const CString *text, int nMaxWidth, int nMaxHeight, const CVector2<int> *BorderSize, bool bUseSpecialChars);
    static int __fastcall GetActualRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, CVector2<short> *NeededSize, bool bUseSpecialChars);
    static void __fastcall GetRequiredSize(CBitmapFont *pFont, int, const CString *OriginalText, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bUseSpecialChars);
    static void __fastcall GetActualRealRequiredSizeActually(CBitmapFont *pFont, int, const CString *Text, CString *NewText, int nMaxWidth, int nMaxHeight, CVector2<int> *BorderSize, bool bWholeWordOnly, bool bAddBreaksToNewText, bool bUseSpecialChars);

    static void InitAndPatch();
};
