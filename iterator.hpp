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

/*
 * This header defines a set of iterator manipulation functions and aliases, 
 * all defined inside the tml::iterator namespace.
 * 
 * The iterator ops are implemented as metafunctions in the tml::iterator::impl
 * namespace, following the common implmentation vs declaration conventions of this 
 * library. 
 * For each function implemented on the tml::iterator::impl namespace there is a corresponsing
 * alias on the tml::iterator namespace which aliases the result of that function.
 * 
 * Also, tml::iterator contains the namespace tml::iterator::func, which defines a set of 
 * metafunctions equivalent to these described above, to allow the user to manipulate and use 
 * iterator manipulation functions on functional expressions.
 */

namespace tml
{
    namespace iterator
    {
        struct forward_iterator_tag{};
        struct reverse_iterator_tag{};
        
        /*
         * This template defines type traits for an iterator type.
         * Any iterator should specify its category throguh this traits.
         */
        template<typename CATEGORY>
        struct iterator_traits
        {
            using iterator_category = CATEGORY;
        };
        
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
        
        /*
         * This namespace defines iterator operations as functions, to be manipulated by the user.
         */
        namespace func
        {
            template<typename I>
            using deref = tml::iterator::impl::next<I>;

            template<typename I>
            using next = tml::iterator::impl::next<I>;
        }

        /*
         * Dereferences the iterator (Obtains the data which the iterator points to)
         */
        template<typename I>
        using deref = typename tml::iterator::impl::deref<I>::result;

        /*
         * Obtains the next iterator of the sequence.
         */
        template<typename I>
        using next = typename tml::iterator::impl::next<I>::result;
    }
}

#endif	/* ITERATOR_HPP */

