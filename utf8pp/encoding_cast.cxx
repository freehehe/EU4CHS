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

#include "encoding_cast.hxx"

#include <iostream>

namespace utf8
{
    namespace platform
    {
        namespace detail
        {
            template <int WCharSize>
            struct converter;

            template <>
            struct converter<4>
            {
                static std::wstring to_wstring(utf8::string const & src)
                {
                    std::wstring result;
                    result.reserve(src.length());
                    for (auto c : src)
                    {
                        result.push_back(c);
                    }
                    return result;
                }

                static utf8::string from_wstring(std::wstring const & src)
                {
                    utf8::string result;
                    for (auto c : src)
                    {
                        result.push_back(c);
                    }
                    return result;
                }
            };

            template <>
            struct converter<2>
            {
                static std::wstring to_wstring(utf8::string const & src)
                {
                    std::wstring result;
                    result.reserve(src.length() * 2);
                    for (auto c : src)
                    {
                        if (c <= 0xFFFF)
                            result.push_back(static_cast<wchar_t>(c));
                        else
                        {
                            // build surrogate pair
                            c -= 0x10000;
                            wchar_t high    = (0x36 << 10) | ((c >> 10) & 0x3FF),
                                    low     = (0x37 << 10) | (c & 0x3FF);
                            result.push_back(high);
                            result.push_back(low);
                        }
                    }
                    return result;
                }

                static utf8::string from_wstring(std::wstring const & src)
                {
                    utf8::string result;
                    value_t val = 0;
                    for (auto c : src)
                    {
                        if ((c >> 10) == 0x36) // start of surrogate
                            val = c & 0x3FF;
                        else if ((c >> 10) == 0x37) // end of surrogate
                        {
                            result.push_back(0x10000 + ((val << 10) | (c & 0x3FF)));
                            val = 0;
                        }
                        else
                            result.push_back(c);
                    }
                    return result;
                }
            };
        }

        utf8::string wstring2utf8(std::wstring const & src)
        {
            return detail::converter<sizeof(wchar_t)>::from_wstring(src);
        }

        std::wstring utf82wstring(utf8::string const & src)
        {
            return detail::converter<sizeof(wchar_t)>::to_wstring(src);
        }
    }
}
