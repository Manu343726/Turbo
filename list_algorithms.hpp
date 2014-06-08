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
            
            template<typename HEAD , typename... TAIL , std::size_t INDEX>
            struct get<tml::list<HEAD,TAIL...>,tml::size_t<INDEX>> : public tml::lists::impl::get<tml::list<TAIL...>,tml::size_t<INDEX-1>>
            {};
            
            template<typename HEAD , typename... TAIL>
            struct get<tml::list<HEAD,TAIL...>,tml::size_t<0>> : public tml::function<HEAD>
            {};
            
            template<typename... ARGS , std::size_t INDEX>
            struct get<tml::list<ARGS...>,tml::size_t<INDEX>>
            {
                static_assert( sizeof...(ARGS) != 0 , "Index out of bounds" );
            };
            
            /* Front operations */
            
            /*
             * Gets the first element of a list.
             * 
             * The complexity of this operation is O(1)
             */
            template<typename LIST>
            struct front;

            template<typename HEAD , typename... TAIL>
            struct front<tml::list<HEAD,TAIL...>> : public tml::function<HEAD>
            {};

            /*
             * Extracts the first element of a list.
             * 
             * The complexity of this operation is O(1)
             */
            template<typename LIST>
            struct pop_front;

            template<typename HEAD , typename... TAIL>
            struct pop_front<tml::list<HEAD,TAIL...>> : public tml::function<tml::list<TAIL...>>
            {};

            /*
             * Inserts an element at the front of a list.
             * 
             * The complexity of this operation is O(1)
             */
            template<typename LIST , typename T>
            struct push_front;

            template<typename... Ts , typename T>
            struct push_front<tml::list<Ts...>,T> : public tml::function<tml::list<T,Ts...>>
            {};
            
            
            
            /* Back operations */
            
            /*
             * Gets the last element of a list.
             * 
             * The complexity of this operation is O(n)
             */
            template<typename LIST , bool end = false>
            struct back;

            template<typename HEAD , typename... TAIL>
            struct back<tml::list<HEAD,TAIL...>,false> : public back<tml::list<TAIL...>,sizeof...(TAIL) <= 1> {};

            template<typename HEAD>
            struct back<tml::list<HEAD>,true> : public tml::function<HEAD> {};

            /*
             * Extracts the last element of a list.
             * 
             * The complexity of this operation is O(n)
             */
            template<typename PROCESSED , typename LIST , bool end = false>
            struct pop_back;

            template<typename... PROCESSED , typename HEAD , typename... TAIL>
            struct pop_back<tml::list<PROCESSED...>,tml::list<HEAD,TAIL...>,false> : public pop_back<tml::list<PROCESSED...,HEAD>,tml::list<TAIL...>,sizeof...(TAIL) <= 1> {};

            template<typename... PROCESSED , typename HEAD>
            struct pop_back<tml::list<PROCESSED...>,tml::list<HEAD>,true> : public tml::function<tml::list<PROCESSED...>> {};

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
        
        namespace func
        {
            template<typename LIST , typename INDEX>
            using get = tml::lists::impl::get<LIST,INDEX>;
        }
        
        template<typename L1 , typename L2>
        using concat = typename tml::lists::impl::concat<L1,L2>::result;
        
        template<typename LIST , typename INDEX>
        using get = typename tml::lists::impl::get<LIST,INDEX>::result;
        
        
        template<typename LIST>
        using back = typename tml::lists::impl::back<LIST,false>::result;

        template<typename LIST>
        using pop_back = typename tml::lists::impl::pop_back<tml::empty_list,LIST,false>::result;

        template<typename LIST , typename T>
        using push_back = typename tml::lists::impl::push_back<LIST,T>::result;
        
        
        template<typename LIST>
        using front = typename tml::lists::impl::front<LIST>::result;

        template<typename LIST>
        using pop_front = typename tml::lists::impl::pop_front<LIST>::result;

        template<typename LIST , typename T>
        using push_front = typename tml::lists::impl::push_front<LIST,T>::result;
    }
}

#endif	/* LIST_ALGORITHMS_HPP */

