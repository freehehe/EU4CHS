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

#ifndef UTF8PP_CORE_TYPES_HXX
#define UTF8PP_CORE_TYPES_HXX

namespace utf8
{
    // value_t represents an index in the unicode table
    typedef unsigned int value_t;
    // byte_t represents a single byte
    typedef unsigned char byte_t;

    // The end of string character
    static value_t const END_OF_STRING = 0;
    // An invalid character
    static value_t const INVALID_CHARACTER = -1;
}

#endif // UTF8PP_CORE_TYPES_HXX
