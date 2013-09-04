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

namespace mpl
{
    
    template<typename LIST , template<typename,typename> class COMPARER = mpl::bigger_than>
    class qsort;
    
    template<template<typename,typename> class COMPARER>
    struct qsort<mpl::empty_list,COMPARER>
    {
        using result = mpl::empty_list;
    };
    
    template<typename T,template<typename,typename> class COMPARER>
    struct qsort<mpl::list<T>,COMPARER>
    {
        using result = mpl::list<T>;
    };
    
    template<typename A , typename B , template<typename,typename> class COMPARER>
    struct qsort<mpl::list<A,B>,COMPARER>
    {
        using result = mpl::conditional<COMPARER<A,B>,mpl::list<A,B>,mpl::list<B,A>>;
    };
    
    template<typename... Ts , template<typename,typename> class COMPARER>
    class qsort<mpl::list<Ts...>,COMPARER>
    {
        template<typename PIVOT , typename LEFT_LIST , typename RIGHT_LIST , typename LIST>
        struct reorder_sublists;
        
        template<typename PIVOT , typename... LEFT_LIST , typename... RIGHT_LIST , typename HEAD , typename... TAIL>
        struct reorder_sublists<PIVOT,mpl::list<LEFT_LIST...>,mpl::list<RIGHT_LIST...>,mpl::list<HEAD,TAIL...>>
        {
            using next_left = mpl::conditional<COMPARER<PIVOT,HEAD>,mpl::list<LEFT_LIST...,HEAD>,mpl::list<LEFT_LIST...>>;
            using next_right = mpl::conditional<COMPARER<PIVOT,HEAD>,mpl::list<RIGHT_LIST...>,mpl::list<HEAD,RIGHT_LIST...>>;
            
            using next_iteration = reorder_sublists<PIVOT,next_left,next_right,mpl::list<TAIL...>>;
            
            using left = typename next_iteration::left;
            using right = typename next_iteration::right;
        };
        
        template<typename PIVOT , typename... LEFT_LIST , typename... RIGHT_LIST>
        struct reorder_sublists<PIVOT,mpl::list<LEFT_LIST...>,mpl::list<RIGHT_LIST...>,mpl::empty_list>
        {
            using left = mpl::list<LEFT_LIST...>;
            using right = mpl::list<RIGHT_LIST...>;
        };
        
        using pivot = mpl::type_at<mpl::list<Ts...>,mpl::size_t<sizeof...(Ts)/2>>;
        using left_sublist = typename reorder_sublists<pivot,mpl::empty_list,mpl::empty_list,mpl::list<Ts...>>::left;
        using right_sublist = typename reorder_sublists<pivot,mpl::empty_list,mpl::empty_list,mpl::list<Ts...>>::right;
        
        using ordered_left  = typename mpl::qsort<left_sublist,COMPARER>::result;
        using ordered_right = typename mpl::qsort<right_sublist,COMPARER>::result;
        
        using concated = mpl::concat<ordered_left,ordered_right>;
        
    public:
        using result = concated;
    };
    
    
    template<typename LIST , template<typename,typename> class COMPARER = mpl::bigger_than>
    using sort = typename qsort<LIST,COMPARER>::result;
}

#endif	/* SORT_HPP */

