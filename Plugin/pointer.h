#pragma once
#include "stdinc.h"

class reveng_pointer
{
	union
	{
		void *_pointer;
		std::uintptr_t _address;
	};

public:
	reveng_pointer(void* pointer)
		: _pointer(pointer)
	{
	}

	reveng_pointer(std::uintptr_t address)
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

	reveng_pointer adjust(std::ptrdiff_t offset) const
	{

	}
};