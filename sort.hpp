/* 
 * File:   sort.hpp
 * Author: paradise
 *
 * Created on 4 de septiembre de 2013, 17:50
 */

#ifndef SORT_HPP
#define	SORT_HPP

#include "list.hpp"
#include "control_flow.hpp"

#include "static_warning.hpp"

namespace mpl
{   
    template<typename LIST , template<typename,typename> class COMPARER = mpl::bigger_than , typename DEBUG_TRACE = mpl::empty_list>
    class qsort;
    
    template<template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<mpl::empty_list,COMPARER,DEBUG_TRACE>
    {
        using result = mpl::empty_list;
        using debug_trace = mpl::list<result>;
    };
    
    template<typename T,template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<mpl::list<T>,COMPARER,DEBUG_TRACE>
    {
        using result = mpl::list<T>;
        using debug_trace = mpl::list<result>;
    };
    
    template<typename A , typename B , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<mpl::list<A,B>,COMPARER,DEBUG_TRACE>
    {
        using result = mpl::conditional<COMPARER<A,B>,mpl::list<A,B>,mpl::list<B,A>>;
        using debug_trace = mpl::list<result>;
    };
    
    template<typename... Ts , template<typename,typename> class COMPARER , typename DEBUG_TRACE>
    struct qsort<mpl::list<Ts...>,COMPARER,DEBUG_TRACE>
    {
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename LEFT_LIST , typename RIGHT_LIST , typename LIST>
        struct reorder_sublists;
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX, typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , INDEX ,mpl::list<LEFT_LIST...>,mpl::list<RIGHT_LIST...>,mpl::list<HEAD,TAIL...>>
        {
            using next_left  = mpl::conditional<COMPARER<HEAD,PIVOT>,mpl::list<LEFT_LIST...,HEAD>,mpl::list<LEFT_LIST...>>;
            using next_right = mpl::conditional<COMPARER<HEAD,PIVOT>,mpl::list<RIGHT_LIST...>,mpl::list<HEAD,RIGHT_LIST...>>;
            
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,INDEX+1,next_left,next_right,mpl::list<TAIL...>>;
            
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT, PIVOT_INDEX , PIVOT_INDEX ,mpl::list<LEFT_LIST...>,mpl::list<RIGHT_LIST...>,mpl::list<HEAD,TAIL...>>
        {
            using next_left = mpl::list<LEFT_LIST...>;
            using next_right = mpl::list<RIGHT_LIST...>;
            
            using next_iteration = reorder_sublists<PIVOT,PIVOT_INDEX,PIVOT_INDEX+1,next_left,next_right,mpl::list<TAIL...>>;
            
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , std::size_t PIVOT_INDEX , std::size_t INDEX , typename... LEFT_LIST , typename... RIGHT_LIST>
        struct reorder_sublists<PIVOT,PIVOT_INDEX,INDEX,mpl::list<LEFT_LIST...>,mpl::list<RIGHT_LIST...>,mpl::empty_list>
        {
            using left = mpl::list<LEFT_LIST...>;
            using right = mpl::list<RIGHT_LIST...>;
        };
        
        using pivot = mpl::type_at<mpl::list<Ts...>,mpl::size_t<sizeof...(Ts)/2>>;
        using left_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,mpl::empty_list,mpl::empty_list,mpl::list<Ts...>>::left;
        using right_sublist = typename reorder_sublists<pivot,sizeof...(Ts)/2,0,mpl::empty_list,mpl::empty_list,mpl::list<Ts...>>::right;
        
        using ordered_left  = typename mpl::qsort<left_sublist,COMPARER,mpl::concat<DEBUG_TRACE,left_sublist>>::result;
        using ordered_right = typename mpl::qsort<right_sublist,COMPARER,mpl::concat<DEBUG_TRACE,right_sublist>>::result;
        
        using ordered_left_debug_trace  = typename mpl::qsort<left_sublist,COMPARER,DEBUG_TRACE>::debug_trace;
        using ordered_right_debug_trace = typename mpl::qsort<right_sublist,COMPARER,DEBUG_TRACE>::debug_trace;
        
        using concated = mpl::concat<ordered_left,mpl::concat<mpl::list<pivot>,ordered_right>>;
        
    public:
        using result = concated;
        using debug_trace = mpl::list<ordered_left_debug_trace,pivot,ordered_right_debug_trace>;
    };
    
    
    template<typename LIST , template<typename,typename> class COMPARER = mpl::bigger_than , template<typename,template<typename,typename>class,typename> class ALGORITHM = mpl::qsort>
    using sort = typename ALGORITHM<LIST,COMPARER,mpl::empty_list>::result;
    
    template<typename LIST , template<typename,typename> class COMPARER = mpl::bigger_than , template<typename,template<typename,typename>class,typename> class ALGORITHM = mpl::qsort>
    using debug_sort = mpl::pair<typename ALGORITHM<LIST,COMPARER,mpl::empty_list>::result,typename ALGORITHM<LIST,COMPARER,mpl::empty_list>::debug_trace>;
}

#endif	/* SORT_HPP */