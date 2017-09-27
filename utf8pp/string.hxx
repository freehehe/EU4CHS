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

#ifndef UTF8PP_CORE_STRING_HXX
#define UTF8PP_CORE_STRING_HXX

#include "types.hxx"
#include "iterator.hxx"

#include <vector>
#include <iosfwd>
#include <string>
#include <cstddef>

namespace utf8
{
    // NOTE: utf-8 string's do not contain a NULL byte while the
    // byte stream used to initialize the string might contain one
    class string
    {
    public:
        // This constructor will throw an std::invalid_argument exception
        // if the input string contains invalid bytes.
        string(char const * str = nullptr);
        ~string();

        string(string const & src);
        string(string && src);
        string & operator = (string const & src);

        explicit string(std::string const & src);

        // get the length of the string in characters
        std::size_t length() const;
        // access the data directly
        std::vector<byte_t> const & data() const;
        std::vector<byte_t> & data();
        // shortcut for data().size()
        std::size_t size() const;

        // iterators
        typedef detail::iterator const_iterator;
        const_iterator begin() const;
        const_iterator end() const;

        // index operator
        value_t operator [] (unsigned index) const;

        // concatenation
        string operator + (string const & other) const;
        string & operator += (string const & other);
        
        // append single characters (might be slow and shouldn't be used too often in perfomance critical code)
        // this method is used in conversions from std::wstring to utf8::string
        void push_back(value_t c);

    private:
        // The byte data of the string
        std::vector<byte_t> data_;
        // The length of the string in characters
        std::size_t length_;
    };

    // uses std::getline and std::string in the background
    std::istream & getline(std::istream & is, string & str, char delim = '\n');
}

// used to write UTF-8 text to an output stream
std::ostream & operator << (std::ostream & os, utf8::string const & str);
// should act just like std::string but with UTF-8 :-)
std::istream & operator >> (std::istream & is, utf8::string & str);

#endif // UTF8PP_CORE_STRING_HXX
