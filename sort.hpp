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
* but WITHOUT ANY WARRANTY; without even the itbied warranty of              * 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU Lesser General Public License for more details.                         *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
******************************************************************************/

#ifndef SORT_HPP
#define	SORT_HPP

#include "list.hpp"
#include "for_loops.hpp"

namespace tml
{   
    template<typename LIST , template<typename,typename> class COMPARER = tml::bigger_than , typename DEBUG_TRACE = tml::empty_list>
    class qsort;
    
    template<template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tml::empty_list,COMPARER,DEBUG_TRACE>
    {
        using result = tml::empty_list;
        using debug_trace = tml::list<result>;
    };
    
    template<typename T,template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tml::list<T>,COMPARER,DEBUG_TRACE>
    {
        using result = tml::list<T>;
        using debug_trace = tml::list<result>;
    };
    
    template<typename A , typename B , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tml::list<A,B>,COMPARER,DEBUG_TRACE>
    {
        using result = tml::conditional<COMPARER<A,B>,tml::list<A,B>,tml::list<B,A>>;
        using debug_trace = tml::list<result>;
    };
    
    template<typename... Ts , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tml::list<Ts...>,COMPARER,DEBUG_TRACE>
    {
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename LEFT_LIST , typename RIGHT_LIST , typename LIST>
        struct reorder_sublists;
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX, typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , INDEX ,tml::list<LEFT_LIST...>,tml::list<RIGHT_LIST...>,tml::list<HEAD,TAIL...>>
        {
            using next_left  = tml::conditional<COMPARER<HEAD,PIVOT>,tml::list<LEFT_LIST...,HEAD>,tml::list<LEFT_LIST...>>;
            using next_right = tml::conditional<COMPARER<HEAD,PIVOT>,tml::list<RIGHT_LIST...>,tml::list<HEAD,RIGHT_LIST...>>;
            
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,INDEX+1,next_left,next_right,tml::list<TAIL...>>;
            
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , PIVOT_INDEX ,tml::list<LEFT_LIST...>,tml::list<RIGHT_LIST...>,tml::list<HEAD,TAIL...>>
        {
            using next_left = tml::list<LEFT_LIST...>;
            using next_right = tml::list<RIGHT_LIST...>;
            
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,PIVOT_INDEX+1,next_left,next_right,tml::list<TAIL...>>;
            
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename... LEFT_LIST , typename... RIGHT_LIST>
        struct reorder_sublists<PIVOT,PIVOT_INDEX,INDEX,tml::list<LEFT_LIST...>,tml::list<RIGHT_LIST...>,tml::empty_list>
        {
            using left = tml::list<LEFT_LIST...>;
            using right = tml::list<RIGHT_LIST...>;
        };
        
        using pivot = tml::type_at<tml::list<Ts...>,tml::size_t<sizeof...(Ts)/2>>;
        using left_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,tml::empty_list,tml::empty_list,tml::list<Ts...>>::left;
        using right_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,tml::empty_list,tml::empty_list,tml::list<Ts...>>::right;
        
        using ordered_left  = typename tml::qsort<left_sublist,COMPARER,tml::concat<DEBUG_TRACE,left_sublist>>::result;
        using ordered_right = typename tml::qsort<right_sublist,COMPARER,tml::concat<DEBUG_TRACE,right_sublist>>::result;
        
        using ordered_left_debug_trace  = typename tml::qsort<left_sublist,COMPARER,DEBUG_TRACE>::debug_trace;
        using ordered_right_debug_trace = typename tml::qsort<right_sublist,COMPARER,DEBUG_TRACE>::debug_trace;
        
        using concated = tml::concat<ordered_left,tml::concat<tml::list<pivot>,ordered_right>>;
        
    public:
        using result = concated;
        using debug_trace = tml::list<ordered_left_debug_trace,pivot,ordered_right_debug_trace>;
    };
    
    
    template<typename LIST , template<typename,typename> class COMPARER = tml::bigger_than , template<typename,template<typename,typename>class,typename> class ALGORITHM = tml::qsort>
    using sort = typename ALGORITHM<LIST,COMPARER,tml::empty_list>::result;
    
    template<typename LIST , template<typename,typename> class COMPARER = tml::bigger_than , template<typename,template<typename,typename>class,typename> class ALGORITHM = tml::qsort>
    using debug_sort = tml::pair<typename ALGORITHM<LIST,COMPARER,tml::empty_list>::result,typename ALGORITHM<LIST,COMPARER,tml::empty_list>::debug_trace>;
}

#endif	/* SORT_HPP */