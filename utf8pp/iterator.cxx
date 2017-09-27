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

#include "iterator.hxx"
#include "string.hxx"

namespace
{
    // calculates the next UTF-8 character of the input byte stream
    // unsafe version -> expects string to be valid
    utf8::value_t next(utf8::byte_t const * & strm, unsigned & length)
    {
        // end of string
        if (!length)
            return utf8::END_OF_STRING;

        utf8::value_t result;
        unsigned charlen;

        // single byte character
        if (!(strm[0] & 0x80))
        {
            charlen = 1;
            result = strm[0] & 0x7F;
        }
        else
        {
            utf8::byte_t mask;
            // determine the byte length of the character
            if ((strm[0] & 0xE0) == 0xC0)
                charlen = 2, mask = 0x1F;
            else if ((strm[0] & 0xF0) == 0xE0)
                charlen = 3, mask = 0x0F;
            else if ((strm[0] & 0xF8) == 0xF0)
                charlen = 4, mask = 0x07;

            // calculate the unicode index
            result = strm[0] & mask;
            for (unsigned byte = 1; byte < charlen; ++byte)
            {
                utf8::byte_t cb = strm[byte];
                result = (result << 6) | (cb & 0x3F);
            }
        }
        // increment ptr and decrement len
        strm += charlen;
        length -= charlen;

        return result;
    }

    utf8::value_t fast_foward(utf8::byte_t const * & strm, unsigned & length, unsigned offset)
    {
        if (!offset)
            return utf8::INVALID_CHARACTER;

        for (unsigned n = 0; n < offset; ++n)
        {
            unsigned step = 0;
            if (!(strm[0] & 0x80))
                step = 1;
            else if ((strm[0] & 0xE0) == 0xC0)
                step = 2;
            else if ((strm[0] & 0xF0) == 0xE0)
                step = 3;
            else if ((strm[0] & 0xF8) == 0xF0)
                step = 4;
            else if (strm[0] == 0)
                return utf8::END_OF_STRING;
            strm += step; length -= step;
        }
        return next(strm, length);
    }
}

namespace utf8
{
    namespace detail
    {
        iterator::iterator()
            : strm_(nullptr)
            , length_(0)
            , value_(INVALID_CHARACTER)
        {
        }

        iterator::~iterator() = default;
        iterator::iterator(iterator const & src) = default;
        iterator & iterator::operator = (iterator const & src) = default;

        iterator::iterator(byte_t const * strm, unsigned length)
            : strm_(strm)
            , length_(length)
            , value_(next(strm, length))
        {
            if (value_ == END_OF_STRING)
                strm_ = nullptr;
        }

        bool iterator::operator == (iterator const & rhs) const
        {
            // it should be sufficient to check if the byte streams are equal
            return (strm_ == rhs.strm_);
        }

        bool iterator::operator != (iterator const & rhs) const
        {
            return (strm_ != rhs.strm_);
        }

        iterator iterator::operator ++ (int)
        {
            iterator tmp(*this);
            value_ = next(strm_, length_);

            if (value_ == END_OF_STRING)
                strm_ = nullptr;

            return tmp;
        }

        iterator & iterator::operator ++ ()
        {
            value_ = next(strm_, length_);

            if (value_ == END_OF_STRING)
                strm_ = nullptr;

            return *this;
        }

        value_t const & iterator::operator * () const
        {
            return value_;
        }

        iterator iterator::operator + (unsigned offset) const
        {
            iterator tmp(*this);
            tmp += offset;
            return tmp;
        }

        iterator & iterator::operator += (unsigned offset)
        {
            value_t tmp = fast_foward(strm_, length_, offset);
            if (tmp != INVALID_CHARACTER)
                value_ = tmp;

            if (value_ == END_OF_STRING)
                strm_ = nullptr;

            return *this;
        }
    }
}
