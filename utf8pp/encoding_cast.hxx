#pragma once

/*
    Copyright (C) 2012, Felix Bytow <felix.bytow@googlemail.com>

    This file is part of utf8pp.

    utf8pp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    utf8pp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with utf8pp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTF8PP_CONVERSION_ENCODINGCAST_HXX
#define UTF8PP_CONVERSION_ENCODINGCAST_HXX

#ifdef UTF8PP_TEST
#include "../core/string.hxx"
#else
#include "string.hxx"
#endif

#include <typeinfo>

namespace utf8
{
    namespace platform
    {
        // platform dependent std::wstring to utf8::string converter (Unix/Linux has 32 bit wchar_t while Windows has 16 bit wchar_t)
        utf8::string wstring2utf8(std::wstring const & src);
        // the opposite conversion
        std::wstring utf82wstring(utf8::string const & src);
    }

    namespace detail
    {
        template <typename TargetType, typename SourceType>
        struct encoding_cast;

        template <typename Type>
        struct encoding_cast<Type, Type>
        {
            static_assert(
                typeid(Type) == typeid(std::string) ||
                typeid(Type) == typeid(std::wstring) ||
                typeid(Type) == typeid(utf8::string)
                , "Unsupported type! Only std::string, std::wstring and utf8::string are allowed."
            );

            static Type convert(Type const & src)
            {
                // same type, no conversion needed
                return src;
            }
        };

        template <>
        struct encoding_cast<utf8::string, std::string>
        {
            static utf8::string convert(std::string const & src)
            {
                return utf8::string(src);
            }
        };

        template <>
        struct encoding_cast<utf8::string, std::wstring>
        {
            static utf8::string convert(std::wstring const & src)
            {
                return platform::wstring2utf8(src);
            }
        };
        
        template <>
        struct encoding_cast<std::wstring, utf8::string>
        {
            static std::wstring convert(utf8::string const & src)
            {
                return platform::utf82wstring(src);
            }
        };
    }

    template <typename TargetType, typename SourceType>
    inline TargetType encoding_cast(SourceType const & src)
    {
        return detail::encoding_cast<TargetType, SourceType>::convert(src);
    }
}

#endif // UTF8PP_CONVERSION_ENCODINGCAST_HXX
