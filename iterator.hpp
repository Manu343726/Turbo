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
        /*
         * Iterators represent sequences on the interval [begin,end),
         * that is, the end iterator points to a "past to end" element
         * of the sequence. 
         * 
         *         begin                       end
         *           |     -------------->      |
         *           v       forward_it         v
         *         +---+------------------+---+  
         *         |   |                  |   |
         *         +---+------------------+---+
         *       ^        backward_it       ^
         *       |     <---------------     |
         *     rend                      rbegin
         * 
         * This 'sequence_end_element' tag is used to
         * represent this value (The element which the end iterator points
         * to).
         */
        struct sequence_end_element{};
        
        /*
         * Tags for iterator categories (Forward or backward iterators)
         */
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
            
            /*
             * Returns a forward iterator pointing to the first element of a sequence.
             */
            template<typename SEQ>
            struct begin;
            
            /*
             * Returns a forward iterator pointing to the end of a sequence.
             */
            template<typename SEQ>
            struct end;
            
            /*
             * Returns a backward iterator pointing to the last element of a sequence.
             */
            template<typename SEQ>
            struct rbegin;
            
            /*
             * Returns a backward iterator pointing to the past-last element of a sequence.
             */
            template<typename SEQ>
            struct rend;
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
            
            template<typename SEQ>
            using begin = tml::iterator::impl::begin<SEQ>;
            
            template<typename SEQ>
            using end = tml::iterator::impl::end<SEQ>;
            
            template<typename SEQ>
            using rbegin = tml::iterator::impl::rbegin<SEQ>;
            
            template<typename SEQ>
            using rend = tml::iterator::impl::rend<SEQ>;
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
        
        /*
         * Returns a forward iterator pointing to the first element of a sequence.
         */
        template<typename SEQ>
        using begin = typename tml::iterator::impl::begin<SEQ>::result;

        /*
         * Returns a forward iterator pointing to the end of a sequence.
         */
        template<typename SEQ>
        using end = typename tml::iterator::impl::end<SEQ>::result;

        /*
         * Returns a backward iterator pointing to the last element of a sequence.
         */
        template<typename SEQ>
        using rbegin = typename tml::iterator::impl::rbegin<SEQ>::result;

        /*
         * Returns a backward iterator pointing to the past-last element of a sequence.
         */
        template<typename SEQ>
        using rend = typename tml::iterator::impl::rend<SEQ>::result;
    }
}

#endif	/* ITERATOR_HPP */

