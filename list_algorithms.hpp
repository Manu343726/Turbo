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

#include "core.hpp"
#include "list.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "for_loops.hpp"

namespace tml
{
    template<typename... Ts>
    struct lenght_t<tml::list<Ts...>> : public tml::function<tml::length<Ts...>>{};
    
    
    template<typename... Ts , typename T>
    struct push_back_t<tml::list<Ts...> , T> : public tml::function<tml::list<Ts...,T>>{};
    
    template<typename... Ts , typename LAST>
    struct pop_back_t<tml::list<Ts...,LAST>> : public tml::function<tml::list<Ts...>>{};
    
    template<typename... Ts , typename T>
    struct push_front<tml::list<Ts...>,T> : public tml::function<tml::list<T,Ts...>>{};
    
    
    
    template<typename... Ts , typename... Us>
    struct concat_t<tml::list<Ts...>,tml::list<Ts...>> : public tml::function<tml::list<Ts...,Us...>>{};
    
    template<typename... Ts , tml::size_t_type INDEX>
    struct split_t<tml::list<Ts...>,tml::size_t<INDEX>>
    {
        template<typename LEFT_LIST , typename RIGHT_LIST>
        struct splitted_list
        {
            using left  = LEFT_LIST;
            using right = RIGHT_LIST;
        };
        
        template<typename T>
        using left = typename T::left;
        
        template<typename T>
        using right = typename T::right;
        
        template<typename PREVIOUS , typename CURRENT>
        struct kernel
        {
            using result = splitted_list<tml::push_back<left<PREVIOUS>,tml::first<right<PREVIOUS>>> , 
                                         tml::pop_back<right<PREVIOUS>>
                                        >;
        };
        
        using result = tml::for_loop<tml::begin<tml::list<Ts...>>                    ,
                                     tml::end<tml::list<Ts...>>                      ,
                                     splitted_list<tml::empty_list,tml::list<Ts...>> ,
                                     kernel
                                    >;
    };
    
    template<typename... Ts , typename T , tml::size_t_type INDEX>
    struct insert_t<tml::list<Ts...> , T , tml::size_t<INDEX>>
    {
        using list = tml::list<Ts...>;
        using index = tml::size_t<INDEX>;
        
        using right = typename tml::split<list,index>::right;
        using left  = typename tml::split<list,index>::left;
        
        using result = tml::concat<tml::push_back<left,T>,right>;
    };
    
    template<typename... Ts , tml::size_t_type INDEX>
    struct erase_t<tml::list<Ts...> , tml::size_t<INDEX>>
    {
        using list = tml::list<Ts...>;
        using index = tml::size_t<INDEX>;
        
        using right = typename tml::split<list,index>::right;
        using left  = typename tml::split<list,index>::left;
        
        using result = tml::concat<left,tml::pop_front<right>>;
    };
}

#endif	/* LIST_ALGORITHMS_HPP */

