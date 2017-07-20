#pragma once
#include "stdinc.h"

extern const HMODULE default_module;

class pattern_byte
{
public:
	enum class match_method :std::uint8_t
	{
		WILDCARD,
		HIGH_FOUR,
		LOW_FOUR,
		EXACT
	};

	pattern_byte()
	{
		this->_method = match_method::WILDCARD;
		this->_value = 0;
	}

	pattern_byte(std::uint8_t value, match_method method = match_method::EXACT)
	{
		this->set(value, method);
	}

	pattern_byte &set(std::uint8_t value, match_method method = match_method::EXACT)
	{
		this->_method = method;
		this->_value = (method == match_method::WILDCARD) ? 0 : value;

		return *this;
	}

	void be_wild()
	{
		this->_method = match_method::WILDCARD;
		this->_value = 0;
	}

	bool is_wild() const
	{
		return this->_method == match_method::WILDCARD;
	}

	bool match(std::uint8_t byte) const
	{
		switch (this->_method)
		{
		case match_method::EXACT:
			return this->_value == byte;

		case match_method::HIGH_FOUR:
			return this->_value == (byte >> 4u);

		case match_method::LOW_FOUR:
			return this->_value == (byte & 0x0Fu);

		case match_method::WILDCARD:
			return true;

		default:
			return false;
		}
	}

	bool compare(const pattern_byte &rhs) const
	{
		return (this->_method == rhs._method) && (this->_value == rhs._value);
	}

	bool operator==(const pattern_byte &rhs) const
	{
		return this->compare(rhs);
	}

	bool operator==(std::uint8_t value) const
	{
		return this->match(value);
	}

	bool operator!=(std::uint8_t value) const
	{
		return !this->match(value);
	}

private:
	std::uint8_t _value;
	match_method _method;
};

class pattern_match
{
	union
	{
		void *_pointer;
		std::uintptr_t _address;
	};

public:
	pattern_match(void* pointer)
		: _pointer(pointer)
	{
	}

	pattern_match(std::uintptr_t address)
		: _address(address)
	{
	}

	std::uintptr_t address(std::ptrdiff_t offset = 0) const
	{
		return (this->_address + offset);
	}

	template<typename T = void>
	T *pointer(std::ptrdiff_t offset = 0) const
	{
		return reinterpret_cast<T *>(this->address(offset));
	}
};

class byte_pattern
{
	std::pair<std::uintptr_t, std::uintptr_t> _range;
	std::vector<pattern_byte> _pattern;
	std::vector<pattern_match> _result;

	bool _processed = false;

	std::array<std::ptrdiff_t, 256> _bmbc;
	std::vector<std::ptrdiff_t> _bmgs;

	void transform_pattern(const char *pattern_literal);
	void executable_range(pattern_match module);

	void initialize(const char *pattern_literal);
	void initialize(pattern_match module, const char *pattern_literal);
	void initialize(pattern_match range_begin, pattern_match range_end, const char *pattern_literal);
	void do_search();

	void bm_preprocess();
	void bm_search();

public:
	explicit byte_pattern(const char *pattern_literal);
	explicit byte_pattern(pattern_match module = default_module, const char *pattern_literal = nullptr);
	byte_pattern(pattern_match range_begin, pattern_match range_end, const char *pattern_literal = nullptr);

	const pattern_match &get(std::size_t index) const;

	byte_pattern &set_pattern(const char *pattern_literal);
	byte_pattern &set_pattern(const void *data, std::size_t size);
	byte_pattern &set_module(pattern_match module = default_module);
	byte_pattern &set_range(pattern_match beg, pattern_match end);
	byte_pattern &force_search();

	std::size_t size() const;
	bool has_size(std::size_t expected) const;
	bool empty() const;
	void clear();

	template <typename Fn>
	void for_each_result(Fn Pr) const
	{
		for (auto &result : this->_result)
		{
			Pr(result);
		}
	}

	bool check_address(std::uintptr_t address) const;
};

extern byte_pattern g_pattern;
