#define NOMINMAX
#include "byte_pattern.h"
#include <windows.h>
#include <array>
#include <algorithm>

extern const HMODULE default_module = GetModuleHandleA(NULL);
byte_pattern g_pattern;

byte_pattern::byte_pattern(const char *pattern_literal)
{
	this->initialize(pattern_literal);
}

byte_pattern::byte_pattern(auto_pointer module, const char *pattern_literal)
{
	this->initialize(module, pattern_literal);
}

byte_pattern::byte_pattern(auto_pointer range_begin, auto_pointer range_end, const char *pattern_literal)
{
	this->initialize(range_begin, range_end, pattern_literal);
}

void byte_pattern::initialize(const char *pattern_literal)
{
	this->executable_range(default_module);
	this->transform_pattern(pattern_literal);
	this->do_search();
}

void byte_pattern::initialize(auto_pointer module, const char *pattern_literal)
{
	this->executable_range(module);
	this->transform_pattern(pattern_literal);
	this->do_search();
}

void byte_pattern::initialize(auto_pointer range_begin, auto_pointer range_end, const char *pattern_literal)
{
	this->set_range(range_begin, range_end);
	this->transform_pattern(pattern_literal);
	this->do_search();
}

const auto_pointer &byte_pattern::get(std::size_t index) const
{
	return this->_result[index];
}

byte_pattern &byte_pattern::set_pattern(const char *pattern_literal)
{
	this->transform_pattern(pattern_literal);

	return *this;
}

byte_pattern &byte_pattern::set_pattern(const void *data, std::size_t size)
{
	this->_pattern.assign(reinterpret_cast<const std::uint8_t *>(data), reinterpret_cast<const std::uint8_t *>(data) + size);
	this->bm_preprocess();

	return *this;
}

byte_pattern &byte_pattern::set_module(auto_pointer module)
{
	this->executable_range(module);

	return *this;
}

byte_pattern &byte_pattern::set_range(auto_pointer beg, auto_pointer end)
{
	this->_range = { beg.address(), end.address() };

	return *this;
}

void byte_pattern::do_search()
{
	if (!this->_processed)
	{
		this->force_search();
	}
}

byte_pattern &byte_pattern::force_search()
{
	this->_result.clear();

	this->_processed = true;

	if (!this->_pattern.empty())
	{
		this->bm_search();
	}

	return *this;
}

void byte_pattern::transform_pattern(const char *pattern_literal)
{
	auto tol = [](char ch) -> uint8_t
	{
		if (ch >= 'A' && ch <= 'F') return uint8_t(ch - 'A' + 10);
		if (ch >= 'a' && ch <= 'f') return uint8_t(ch - 'a' + 10);
		return uint8_t(ch - '0');
	};

	auto is_digit = [](char ch) -> bool
	{
		return (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f') || (ch >= '0' && ch <= '9');
	};

	std::array<char, 2> temp_string{ 0, 0 };

	this->_pattern.clear();

	if (pattern_literal == nullptr)
	{
		return;
	}

	const char *patit = pattern_literal;
	const char *patend = (pattern_literal + std::strlen(pattern_literal) + 1);

	while (patit != patend)
	{
		char ch = *patit;

		if (ch == ' ' || ch == 0)
		{
			if (!temp_string[0] && !temp_string[1])
			{
				continue;
			}
			else if (temp_string[0] == '?' && (temp_string[1] == '?' || temp_string[1] == 0))
			{
				this->_pattern.emplace_back();
			}
			else if (temp_string[0] == '?' && is_digit(temp_string[1]))
			{
				this->_pattern.emplace_back(tol(temp_string[1]), pattern_byte::match_method::LOW_FOUR);
			}
			else if (temp_string[1] == '?' && is_digit(temp_string[0]))
			{
				this->_pattern.emplace_back(tol(temp_string[0]), pattern_byte::match_method::HIGH_FOUR);
			}
			else if (is_digit(temp_string[0]) && is_digit(temp_string[1]))
			{
				this->_pattern.emplace_back((tol(temp_string[0]) << 4) | tol(temp_string[1]));
			}
			else
			{
				this->_pattern.clear();
				return;
			}

			temp_string.fill(0);
		}
		else
		{
			if (temp_string[0] == 0)
			{
				temp_string[0] = ch;
			}
			else if (temp_string[1] == 0)
			{
				temp_string[1] = ch;
			}
			else
			{
				this->_pattern.clear();
				return;
			}
		}

		++patit;
	}

	this->bm_preprocess();
}

void byte_pattern::executable_range(auto_pointer module)
{
	static auto getSection = [](const PIMAGE_NT_HEADERS nt_headers, unsigned section) -> PIMAGE_SECTION_HEADER
	{
		return reinterpret_cast<PIMAGE_SECTION_HEADER>(
			(UCHAR*)nt_headers->OptionalHeader.DataDirectory +
			nt_headers->OptionalHeader.NumberOfRvaAndSizes * sizeof(IMAGE_DATA_DIRECTORY) +
			section * sizeof(IMAGE_SECTION_HEADER));
	};

	this->_range.first = module.address();

	PIMAGE_DOS_HEADER dosHeader = module.pointer<IMAGE_DOS_HEADER>();
	PIMAGE_NT_HEADERS ntHeader = module.pointer<IMAGE_NT_HEADERS>(dosHeader->e_lfanew);

	for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++)
	{
		auto sec = getSection(ntHeader, i);
		auto secSize = sec->SizeOfRawData != 0 ? sec->SizeOfRawData : sec->Misc.VirtualSize;
		if (sec->Characteristics & IMAGE_SCN_MEM_EXECUTE)
			this->_range.second = this->_range.first + sec->VirtualAddress + secSize;

		if ((i == ntHeader->FileHeader.NumberOfSections - 1) && this->_range.second == 0)
			this->_range.second = this->_range.first + sec->PointerToRawData + secSize;
	}
}

void byte_pattern::clear()
{
	_range = { 0,0 };
	this->_pattern.clear();
	this->_result.clear();
	this->_processed = false;
}

std::size_t byte_pattern::size() const
{
	return this->_result.size();
}

bool byte_pattern::has_size(std::size_t expected) const
{
	return (this->_result.size() == expected);
}

bool byte_pattern::empty() const
{
	return this->_result.empty();
}

bool byte_pattern::check_address(std::uintptr_t address) const
{
	return std::equal(this->_pattern.begin(), this->_pattern.end(), reinterpret_cast<const uint8_t *>(address));
}

void byte_pattern::bm_preprocess()
{
	std::ptrdiff_t index;

	//Bad characters
	for (std::uint32_t bc = 0; bc < 256; ++bc)
	{
		for (index = this->_pattern.size() - 1; index >= 0; --index)
		{
			if (this->_pattern[index].match(bc))
			{
				break;
			}
		}

		this->_bmbc[bc] = index;
	}

	//Good suffix
	std::ptrdiff_t m = this->_pattern.size();

	this->_bmgs.resize(m, 1);

	std::vector<std::ptrdiff_t> suffix(m);

	suffix[m - 1] = m;

	for (auto i = m - 2; i >= 0; --i)
	{
		auto q = i;

		while (q >= 0 && this->_pattern[q] == this->_pattern[m - 1 - i + q])
			--q;

		suffix[i] = i - q;
	}

	for (auto i = 0; i < m; ++i)
		this->_bmgs[i] = m;

	auto j = 0;

	for (auto i = m - 1; i >= 0; --i)
		if (suffix[i] == i + 1)
			for (; j < m - 1 - i; ++j)
				if (this->_bmgs[j] == m)
					this->_bmgs[j] = m - 1 - i;

	for (auto i = 0; i <= m - 2; ++i)
		this->_bmgs[m - 1 - suffix[i]] = m - 1 - i;
}

void byte_pattern::bm_search()
{
	std::uint8_t *range_begin = reinterpret_cast<std::uint8_t *>(this->_range.first);
	std::uint8_t *range_end = reinterpret_cast<std::uint8_t *>(this->_range.second - this->_pattern.size());

	std::ptrdiff_t index;

	auto pattern_length = this->_pattern.size();

	__try
	{
		while (range_begin <= range_end)
		{
			for (index = pattern_length - 1; index >= 0; --index)
			{
				if (!this->_pattern[index].match(range_begin[index]))
				{
					break;
				}
			}

			if (index == -1)
			{
				this->_result.emplace_back(range_begin);
				range_begin += pattern_length;
			}
			else
			{
				range_begin += std::max(index - this->_bmbc[range_begin[index]], this->_bmgs[index]);
			}
		}
	}
	__except ((GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{

	}
}
