/******************************************************************************
 * Turbo C++11 metaprogramming Library                                         *
 *                                                                             *
 * Copyright (C) 2013 - 2014, Manuel Sánchez Pérez                             *
 *                                                                             *
 * This file is part of The Turbo Library.                                     *
 *                                                                             *
 * The Turbo Library is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, version 2 of the License.                     *
 *                                                                             *
 * The Turbo Library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#ifndef ITERATOR_HPP
#define	ITERATOR_HPP

namespace tml
{
    namespace iterator
    {
        namespace impl
        {
            /*
             * Obtains the data pointed by an iterator.
             * 
             * This operation must be O(1).
             */
            template<typename I>
            struct deref;
            
            /*
             * Obtains the iterator pointing to the next element of the sequence.
             */
            template<typename I>
            struct next;
        }
    }
}

#endif	/* ITERATOR_HPP */

