#include "functions.h"
#include "eu4.h"

using namespace std;

namespace Functions
{
	void __fastcall ConvertUTF8ToLatin1(const char *source, char *dest)
	{
		string_view source_view(source);

		vector<uint32_t> wideText;
		utf8::utf8to32(source_view.begin(), source_view.end(), back_inserter(wideText));

		for (uint32_t &cp : wideText)
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

			default:
				break;
			}
		}

		wideText.push_back(0);
		eu4utf8::utf32to8(wideText.begin(), wideText.end(), dest);
	}

	bool IsLatin1Char(uint32_t cp)
	{
		return cp <= 0xFF;
	}

	bool IsTextIconChar(uint32_t cp)
	{
		return isalpha(cp) || isdigit(cp) || cp == '_' || cp == '|';
	}
	
	void GetTwoUnicode(const char * pText, uint32_t index, uint32_t text_length, uint32_t &first, ptrdiff_t &first_length, uint32_t &second, bool bUseSpecialChars)
	{
		second = 0;

		//First
		if (index < text_length)
		{
			first_length = eu4utf8::internal::sequence_length(pText + index);
			first = eu4utf8::peek_next(pText + index, pText + text_length);
			index += first_length;
		}

		//Second
		if (index < text_length)
		{
			ptrdiff_t second_length = eu4utf8::internal::sequence_length(pText + index);
			uint32_t temp_second = eu4utf8::peek_next(pText + index, pText + text_length);

			if (bUseSpecialChars)
			{
				while (temp_second == 0xA7)
				{
					index += 2;

					if (index >= text_length)
					{
						return;
					}

					temp_second = eu4utf8::peek_next(pText + index, pText + text_length);
				}
			}

			second = temp_second;
		}
	}

	void GetTwoUnicode(std::vector<uint32_t>::iterator pText, std::vector<uint32_t>::iterator pEnd, uint32_t & first, uint32_t & second, bool bUseSpecialChars)
	{
		second = 0;

		//First
		if (pText < pEnd)
		{
			first = *pText;
			++pText;
		}

		//Second
		if (pText < pEnd)
		{
			uint32_t temp_second = *pText;

			if (bUseSpecialChars)
			{
				while (temp_second == 0xA7)
				{
					pText += 2;

					if (pText >= pEnd)
					{
						return;
					}

					temp_second = *pText;
				}
			}

			second = temp_second;
		}
	}

	struct CSdlEvents_HandlePdxEvents_0x2DE
	{
		void operator()(injector::reg_pack &regs) const
		{
			CInputEvent temp;
			char *pText = (char *)(regs.ebp.i - 0x48);

			string_view text_view(pText);

			for (char c : text_view)
			{
				temp.Init(c);

				injector::thiscall<void(void *, const CInputEvent *)>::vtbl<3>(regs.ebx, &temp);
			}

			memset(pText, 0, 32);

			regs.ecx.i = 0;
			regs.eax.i = 0;
		}
	};

	void InitAndPatch()
	{
		//yml转码函数
		g_pattern.find_pattern("81 EC B0 00 00 00 53 56 8B F1 8B DA");
		if (g_pattern.has_size(1))
			injector::MakeJMP(g_pattern.get_first().integer(-0x18), ConvertUTF8ToLatin1);

		//从输入接受整个字符串
		//mov ecx, [ebp - 0x48]; xor al, al
		g_pattern.find_pattern("8B 4D B8 32 C0");
		if (g_pattern.has_size(1))
			injector::MakeInline<CSdlEvents_HandlePdxEvents_0x2DE>(g_pattern.get_first().integer());

		//校验总是成功
		g_pattern.find_pattern("0F 94 45 F3 56");
		if (g_pattern.has_size(1))
			injector::MakeNOP(g_pattern.get_first().integer(), 4, true);

		//贴图大小限制
		g_pattern.find_pattern("81 FE 00 00 00 01 72 0F");
		if (g_pattern.has_size(1))
			injector::WriteMemory<uint8_t>(g_pattern.get_first().integer(6), 0xEB, true);
	}
}
