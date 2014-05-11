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

#ifndef LIST_ALGORITHMS_HPP
#define	LIST_ALGORITHMS_HPP

#include "list.hpp"
#include "indices_trick.hpp"
#include "function.hpp"


/*
 * This header defines a set of useful algorithms to manipulate lists, such as
 * searching, concating, splitting, etc.
 */

namespace tml
{   
    namespace lists
    {
        namespace impl
        {
            /*
             * Concatenates two lists
             * 
             * This operation has O(1) complexity due to pattern matching.
             */
            template<typename L1 , typename L2>
            struct concat;
            
            template<typename... Ts , typename... Us>
            struct concat<tml::list<Ts...>,tml::list<Us...>> : public tml::function<tml::list<Ts...,Us...>>
            {};
            
            /*
             * Gets the element at the specified position of a list.
             * 
             * This operation has O(logN) complexity: The indices trick is used 
             * to perform binary search of the index, reducing the number of required
             * template instantations to log_2(N)
             */
            template<typename LIST , typename INDEX>
            struct get;
            
            template<typename... Ts , typename INDEX>
            
            
            
            /*
             * Gets the last element of a list.
             * 
             * The complexity of this operation is O(1) (Works through pattern matching)
             */
            template<typename LIST>
            struct back;

            template<typename... Ts , typename BACK>
            struct back<tml::list<Ts...,BACK>> : public tml::function<BACK>
            {};

            /*
             * Extracts the last element of a list.
             * 
             * The complexity of this operation is O(1) (Works through pattern matching)
             */
            template<typename LIST>
            struct pop_back;

            template<typename... Ts , typename BACK>
            struct pop_back<tml::list<Ts...,BACK>> : public tml::function<tml::list<Ts...>>
            {};

            /*
             * Inserts an element at the end of a list.
             * 
             * The complexity of this operation is O(1) (Works through pattern matching)
             */
            template<typename LIST , typename T>
            struct push_back;

            template<typename... Ts , typename T>
            struct push_back<tml::list<Ts...>,T> : public tml::function<tml::list<Ts...,T>>
            {};
        }
        
        template<typename L1 , typename L2>
        using concat = typename tml::lists::impl::concat<L1,L2>::result;
        
        template<typename LIST>
        using back = typename tml::lists::impl::pop_back<LIST>::result;

        template<typename LIST>
        using pop_back = typename tml::lists::impl::pop_back<LIST>::result;

        template<typename LIST , typename T>
        using push_back = typename tml::lists::impl::push_back<LIST,T>::result;
    }
}

#endif	/* LIST_ALGORITHMS_HPP */

