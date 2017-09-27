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

#ifndef UTF8PP_CORE_ITERATOR_HXX
#define UTF8PP_CORE_ITERATOR_HXX

#include "types.hxx"

namespace utf8
{
    class string;

    namespace detail
    {
        class iterator
        {
        public:
            iterator();
            ~iterator();

            iterator(iterator const & src);
            iterator & operator = (iterator const & src);

            bool operator == (iterator const & rhs) const;
            bool operator != (iterator const & rhs) const;

            iterator operator ++ (int);
            iterator & operator ++ ();

            iterator operator + (unsigned offset) const;
            iterator & operator += (unsigned offset);

            value_t const & operator * () const;

        private:
            iterator(byte_t const * strm, unsigned length);

            // same data as used by internal next and next_s
            byte_t const * strm_;
            unsigned length_;

            // the current value
            value_t value_;

            // string class needs access to the constructor
            friend class ::utf8::string;
        };
    }
}

#endif // UTF8PP_CORE_ITERATOR_HXX
