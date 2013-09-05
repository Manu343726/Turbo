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
    template<typename LIST , template<typename,typename> class COMPARER>
    class qsort
    {
    private:
        //Forward declaration:
        template<std::size_t lenght , typename SUBLIST>
        struct _qsort;

        //Base-case for empty sublists:
        template<typename... Ns>
        struct _qsort<0,mpl::list<Ns...>>
        {
            using result = mpl::empty_list;
        };

        //Base case for one element sublists:
        template<typename UINT_WRAPPER>
        struct _qsort<1,mpl::list<UINT_WRAPPER>>
        {
            using result = mpl::list<UINT_WRAPPER>;
        };

        //Base-case for two elements sublists (Simple compare and swap):
        template<typename FIRST , typename LAST >
        struct _qsort<2,mpl::list<FIRST,LAST>>
        {
            using result = mpl::conditional< COMPARER<FIRST,LAST> ,               //CONDITION
                                                mpl::list<FIRST,LAST>,            //THEN
                                                mpl::list<LAST,FIRST>             //ELSE
                                              >;
        };

        //Recursive case:
        template<std::size_t lenght , typename... Ns>
        struct _qsort<lenght,mpl::list<Ns...>>
        {
        private:
            /* STEP 1: Reorder the sublist in two sublists: Left sublist, with elements greater than to pivot, and right, with the others */

            //Forward declaration:
            template<typename PIVOT , typename RIGHT /* initial (or actual) right sublist */ , typename LEFT /* initial (or actual) left sublist */ , typename _UINT_LIST /* the sublist */>
            struct _reorder_sublists;

            //Recursive case:
            template<typename PIVOT , typename... RIGHT_UINTS , typename... LEFT_UINTS , typename HEAD , typename... TAIL>
            struct _reorder_sublists<PIVOT,mpl::list<RIGHT_UINTS...>,mpl::list<LEFT_UINTS...>,mpl::list<HEAD,TAIL...>>
            {
                using _next_left  = mpl::list<LEFT_UINTS...,HEAD>;  ///< Next left  sublist if HEAD is greather than PIVOT.
                using _next_right = mpl::list<HEAD,RIGHT_UINTS...>; ///< Next right sublist if HEAD is less than PIVOT.
                //                                        CONDITION             THEN                    ELSE
                using next_left  = mpl::conditional< mpl::logical_not<COMPARER<PIVOT,HEAD>> , _next_left  , mpl::list<LEFT_UINTS...>>;
                using next_right = mpl::conditional<  COMPARER<PIVOT,HEAD> , _next_right , mpl::list<RIGHT_UINTS...>>;

                using next_reorder = _reorder_sublists<PIVOT,next_right,next_left,mpl::list<TAIL...>>; // "Recursive call" (Iteration)

                using right = typename next_reorder::right; //Recursive result return
                using left  = typename next_reorder::left;  //Recursive result return
            };

            //Base case (End of the iteration):
            template<typename PIVOT , typename RIGHT , typename LEFT>
            struct _reorder_sublists<PIVOT,RIGHT,LEFT,mpl::empty_list>
            {
                using right = RIGHT;
                using left  = LEFT;
            };

            template<typename PIVOT>
            using _right_sublist = typename _reorder_sublists<PIVOT,mpl::empty_list,mpl::empty_list,mpl::list<Ns...>>::right; //Right sublist computation
            template<typename PIVOT>
            using _left_sublist  = typename _reorder_sublists<PIVOT,mpl::empty_list,mpl::empty_list,mpl::list<Ns...>>::left;  //Left sublist computation

        private:
            static const unsigned int _half_size   = lenght/2;
            static const unsigned int _pivot_index = _half_size; //"Silly" pivot policy. Random-pivot instead? http://stackoverflow.com/questions/11498304/generate-random-numbers-in-c-at-compile-time
            using _pivot = mpl::type_at<mpl::list<Ns...>,mpl::size_t<_pivot_index>>;

            using _right = _right_sublist<_pivot>; //"Call" to reordered right sublist computation
            using _left  = _left_sublist<_pivot>;  //"Call" to reordered left sublist computation

        public:
            /* STEP 2: Recursive "call" to qsort passing the two generated sublists */

            using result = mpl::concat< typename _qsort< _left::size , _left >::result , typename _qsort< _right::size , _right >::result >;
        };

    public:
        using result = typename _qsort<LIST::size,LIST>::result; //"Call" to qsort computation;
    };
    
    template<typename LIST , template<typename,typename> class COMPARER = mpl::less_than , template<typename,template<typename,typename> class> class ALGORITHM = mpl::qsort>
    using sort = typename ALGORITHM<LIST,COMPARER>::result;
}

#endif	/* SORT_HPP */

