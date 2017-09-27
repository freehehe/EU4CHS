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

#include "string.hxx"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>

#include <cstring>
#include <cmath>

namespace
{
    // calculates the next UTF-8 character of the input byte stream
    // safe version -> checks for invalid characters
    utf8::value_t next_s(utf8::byte_t const * strm = nullptr, unsigned length = -1)
    {
        static utf8::byte_t const * ptr = nullptr;
        static unsigned len = 0;
        if (strm)
        {
            ptr = strm;
            len = length;
        }

        // end of string
        if (!len)
            return utf8::END_OF_STRING;

        utf8::value_t result;
        unsigned charlen;

        // single byte character
        if (!(ptr[0] & 0x80))
        {
            charlen = 1;
            result = ptr[0] & 0x7F;
        }
        else
        {
            utf8::byte_t mask;
            // determine the byte length of the character
            if ((ptr[0] & 0xE0) == 0xC0)
                charlen = 2, mask = 0x1F;
            else if ((ptr[0] & 0xF0) == 0xE0)
                charlen = 3, mask = 0x0F;
            else if ((ptr[0] & 0xF8) == 0xF0)
                charlen = 4, mask = 0x07;
            else
                return utf8::INVALID_CHARACTER;

            // character would need more bytes than available
            if (charlen > len)
                return utf8::INVALID_CHARACTER;

            // calculate the unicode index
            result = ptr[0] & mask;
            for (unsigned byte = 1; byte < charlen; ++byte)
            {
                utf8::byte_t cb = ptr[byte];
                if ((cb & 0xC0) != 0x80)
                    return utf8::INVALID_CHARACTER;
                result = (result << 6) | (cb & 0x3F);
            }
        }
        // increment ptr and decrement len
        ptr += charlen;
        len -= charlen;

        return result;
    }
}

namespace utf8
{
    string::string(char const * str /* = nullptr */)
        : length_(0)
    {
        if (str)
        {
            // calculate buffer size
            unsigned bufflen = static_cast<unsigned>(std::strlen(str));

            // calculate length of string
            value_t val = ::next_s(reinterpret_cast<byte_t const *>(str), bufflen);
            while ((val != INVALID_CHARACTER) && (val != END_OF_STRING))
            {
                ++length_;
                val = ::next_s();
            }

            // error handling
            if (val == INVALID_CHARACTER)
                throw std::invalid_argument("String contains non UTF-8 bytes.");

            // copy string
            data_.resize(bufflen);
            std::memcpy(&(data_[0]), str, bufflen);
        }
    }

    string::~string() = default;
    string::string (string const & src) = default;
    string & string::operator = (string const & src) = default;

    string::string(std::string const & src)
        : length_(0)
        , data_(std::begin(src), std::end(src))
    {
        unsigned bufflen = static_cast<unsigned>(data_.size());
        value_t val = ::next_s(&(data_[0]), bufflen);
        while ((val != INVALID_CHARACTER) && (val != END_OF_STRING))
        {
            ++length_;
            val = ::next_s();
        }

        // error handling
        if (val == INVALID_CHARACTER)
            throw std::invalid_argument("String contains non UTF-8 bytes.");
    }

    // move constructor
    string::string(string && src)
        : length_(src.length_)
    {
        data_.swap(src.data_);
    }

    std::size_t string::length() const
    {
        return length_;
    }

    std::vector<byte_t> const & string::data() const
    {
        return data_;
    }

    std::vector<byte_t> & string::data()
    {
        return data_;
    }
    
    std::size_t string::size() const
    {
        return data_.size();
    }

    string::const_iterator string::begin() const
    {
        return const_iterator(&(data_[0]), static_cast<unsigned>(data_.size()));
    }

    string::const_iterator string::end() const
    {
        return const_iterator();
    }

    value_t string::operator [] (unsigned index) const
    {
        if (index > length_)
            throw std::range_error("Index is too big.");
        return *(begin() + index);
    }

    string string::operator + (string const & other) const
    {
        string cpy(*this);
        cpy += other;
        return cpy;
    }

    string & string::operator += (string const & other)
    {
        unsigned oldsize = static_cast<unsigned>(data_.size());
        length_ += other.length_;
        data_.resize(data_.size() + other.data_.size());
        std::memcpy(&(data_[oldsize]), &(other.data_[0]), other.data_.size());
        return *this;
    }
    
    void string::push_back(value_t c)
    {
        unsigned numbits = static_cast<unsigned>(std::ceil(std::log2(c)));
        if (numbits > 21)
            throw std::invalid_argument("c is not a valid Unicode character.");
        
        unsigned numbytes = 0; // number of UTF-8 Bytes
        if (numbits <= 7)
            numbytes = 1;
        else if (numbits > 7 && numbits <= 11)
            numbytes = 2;
        else if (numbits > 11 && numbits <= 16)
            numbytes = 3;
        else
            numbytes = 4;
        
        if (!numbytes) // no null-bytes in utf8::string
            return;
        
        std::vector<byte_t> character(numbytes);
        
        for (unsigned byte = numbytes - 1; byte >= 1; --byte)
        {
            character[byte] = 0x80 | (c & 0x3F);
            c >>= 6;
        }
        
        switch (numbytes)
        {
            case 1: character[0] = c & 0x7F; break;
            case 2: character[0] = 0xC0 | (c & 0x1F); break;
            case 3: character[0] = 0xE0 | (c & 0x0F); break;
            case 4: character[0] = 0xF0 | (c & 0x07); break;
        }
        
        // append the new character
        data_.insert(data_.end(), character.begin(), character.end());
        ++length_;
    }

    std::istream & getline(std::istream & is, string & str, char delim /* = '\n' */)
    {
        std::string tmp;
        std::getline(is, tmp, delim);
        str = utf8::string(tmp);
        return is;
    }
}

std::ostream & operator << (std::ostream & os, utf8::string const & str)
{
    return os.write(reinterpret_cast<char const *>(&((str.data())[0])), str.data().size());
}

std::istream & operator >> (std::istream & is, utf8::string & str)
{
    std::string tmp;
    is >> tmp;
    str = utf8::string(tmp);
    return is;
}
