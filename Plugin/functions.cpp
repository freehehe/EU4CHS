#include "stdinc.h"
#include "functions.h"
#include "eu4.h"

void __fastcall CGlobalFunctions::ConvertUTF8ToLatin1(const char *source, char *dest)
{
	static std::vector<uint32> u32sequence;	

	std::string_view source_view(source);
	u32sequence.clear();
	utf8::unchecked::utf8to32(source_view.begin(), source_view.end(), std::back_inserter(u32sequence));

	for (uint32 &cp : u32sequence)
	{
		switch (cp)
		{
		case 0x80:
		case 0x82:
		case 0x83:
		case 0x84:
		case 0x85:
		case 0x86:
		case 0x87:
		case 0x88:
		case 0x89:
		case 0x8a:
		case 0x8b:
		case 0x8c:
		case 0x8e:
		case 0x91:
		case 0x92:
		case 0x93:
		case 0x94:
		case 0x95:
		case 0x96:
		case 0x97:
		case 0x98:
		case 0x99:
		case 0x9a:
		case 0x9b:
		case 0x9c:
		case 0x9e:
		case 0x9f:
			cp = '?';
			break;

		case 0x152: cp = 0x8c; break;
		case 0x153: cp = 0x9c; break;
		case 0x160: cp = 0x8a; break;
		case 0x161: cp = 0x9a; break;
		case 0x178: cp = 0x9f; break;
		case 0x17d: cp = 0x8e; break;
		case 0x17e: cp = 0x9e; break;
		case 0x192: cp = 0x83; break;
		case 0x2c6: cp = 0x88; break;
		case 0x2dc: cp = 0x98; break;
		case 0x2013: cp = 0x96; break;
		case 0x2014: cp = 0x97; break;
		case 0x2018: cp = 0x91; break;
		case 0x2019: cp = 0x92; break;
		case 0x201a: cp = 0x82; break;
		case 0x201c: cp = 0x93; break;
		case 0x201d: cp = 0x94; break;
		case 0x201e: cp = 0x84; break;
		case 0x2020: cp = 0x86; break;
		case 0x2021: cp = 0x87; break;
		case 0x2022: cp = 0x95; break;
		case 0x2026: cp = 0x85; break;
		case 0x2030: cp = 0x89; break;
		case 0x2039: cp = 0x8b; break;
		case 0x203a: cp = 0x9b; break;
		case 0x20ac: cp = 0x80; break;
		case 0x2122: cp = 0x99; break;

		default:
			break;
		}
	}

	u32sequence.push_back(0);
	utf8::utf32to8(u32sequence.begin(), u32sequence.end(), dest);
}

bool CGlobalFunctions::IsNativeCharacter(uint32 cp)
{
	return cp <= 0xFF;
}

bool CGlobalFunctions::IsTextIconChar(uint32 cp)
{
	return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
}

void CGlobalFunctions::Patch()
{
	injector::MakeJMP(game.pfConvertUTF8ToLatin1, ConvertUTF8ToLatin1);
}
