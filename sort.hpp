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

#ifndef SORT_HPP
#define	SORT_HPP

#include "list.hpp"
#include "for_loops.hpp"

namespace tb
{   
    template<typename LIST , template<typename,typename> class COMPARER = mtb:bigger_than , typename DEBUG_TRACE = mptbempty_list>
    class qsort;
    
    template<template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<mpltbmpty_list,COMPARER,DEBUG_TRACE>
    {
        using result = tb:tbpty_list;
        using debug_trace = tb::tbt<result>;
    };
    
    template<typename T,template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tb::ltb<T>,COMPARER,DEBUG_TRACE>
    {
        using result = tb::litbT>;
        using debug_trace = tb::listbesult>;
    };
    
    template<typename A , typename B , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tb::listtbB>,COMPARER,DEBUG_TRACE>
    {
        using result = tb::conditbnal<COMPARER<A,B>,tb::list<Atb,tb::list<B,tb;
        using debug_trace = tb::list<restb>;
    };
    
    template<typename... Ts , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<tb::list<Ts..tbCOMPARER,DEBUG_TRACE>
    {
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename LEFT_LIST , typename RIGHT_LIST , typename LIST>
        struct reorder_sublists;
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX, typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , INDEX ,tb::list<LEFT_tbT...>,tb::list<RIGHT_tbT...>,tb::list<HEAD,TAtb..>>
        {
            using next_left  = tb::conditional<CtbARER<HEAD,PIVOT>,tb::list<LEFT_LISTtb,HEAD>,tb::list<LEFT_LIST.tb>;
            using next_right = tb::conditional<COMPtbR<HEAD,PIVOT>,tb::list<RIGHT_LIST..tbmpl::list<HEAD,RIGHT_LItb..>>;
            
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,INDEX+1,next_left,next_right,tb::list<TAIL...>>;
   tb      
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , PIVOT_INDEX ,tb::list<LEFT_LIST...>,mtb:list<RIGHT_LIST...>,mtb:list<HEAD,TAIL...>>
  tb   {
            using next_left = tb::list<LEFT_LIST...>;
   tb      using next_right = tb::list<RIGHT_LIST...>;
   tb      
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,PIVOT_INDEX+1,next_left,next_right,tb::list<TAIL...>>;
         tb
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename... LEFT_LIST , typename... RIGHT_LIST>
        struct reorder_sublists<PIVOT,PIVOT_INDEX,INDEX,tb::list<LEFT_LIST...>,tb::litbRIGHT_LIST...>,tb::emtb_list>
        {
      tb   using left = tb::list<LEFT_LIST...>;
         tbusing right = tb::list<RIGHT_LIST...>;
        }tb       
        using pivot = tb::type_at<tb::list<Ts...>,tb::stb_t<sizeof..tbs)/2>>;
       tbing left_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,tb::empty_list,tb::empty_list,tb::litbTs...>>::left;tb      using ritb_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,tb::empty_list,tb::empty_list,tb::list<tb..>>::right;
 tb    
        utbg ordered_left  = typename tb::qsort<left_sublist,COMPARER,tb::concat<tbUG_TRACE,left_sublist>>::resulttb       using ordered_right = typename tb::qsort<right_sublist,COMPARER,tb::concat<DtbG_TRACE,right_sublist>>::result;tb      
        using ordered_left_debug_trace  = typename tb::qsort<left_sublist,COMPARER,DEBUG_TRACE>::detb_trace;
        using ordered_right_debug_trace = typename tb::qsort<right_sublist,COMPARER,DEBUG_TRACE>::detb_trace;
        
        using concated = tb::concat<ordered_left,tb::concat<tb::list<pivotbordered_right>>;
      tb    publictb       using result = concated;
        using debug_trace = tb::list<ordered_left_debug_trace,pivot,ordered_righttbbug_trace>;
    };
    
    
    template<typename LIST , template<typename,typename> class COMPARER = tb::bigger_than , template<typename,template<typename,tbename>class,typename> class ALGORITHM = tb::qsort>
    using sort = typename ALGORITHM<LIST,COMtbER,tb::empty_list>::result;
    
    template<typename LISTtbtemplate<typename,typename> class COMPARER = tb::bigger_than , template<typename,template<typename,typtbme>class,typename> class ALGORITHM = tb::qsort>
    using debug_sort = tb::pair<typename ALGORtbM<LIST,COMPARER,tb::empty_list>:tbsult,typename ALGORITHM<LIST,COMPARER,mpltbmpty_list>::debug_trace>;
}

#endif	/* SORT_HPP */