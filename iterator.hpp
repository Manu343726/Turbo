/* 
 * File:   iterator.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 19:38
 */

#ifndef ITERATOR_HPP
#define	ITERATOR_HPP

#include "list.hpp"
#include "operators.hpp"
#include "expressions.hpp"


namespace mpl
{
    template<typename VALUE>
    struct iterator
    {
        using value = VALUE;
    };
    
    template<typename ITERATOR>
    using dereference = typename ITERATOR::value;

    template<typename VALUE , typename RIGHT>
    struct forward_iterator : public iterator<VALUE>
    {
        using right = RIGHT;
    };

    template<typename LEFT , typename VALUE>
    struct backward_iterator : public iterator<VALUE>
    {
        using left = LEFT;
    };

    template<typename LEFT , typename VALUE , typename RIGHT>
    struct bidirectional_iterator : public iterator<VALUE>
    {
        using right = RIGHT;
        using left = LEFT;
    };
    
    
    template<typename VALUE , typename RIGHT>
    using make_forward_iterator = mpl::function<mpl::forward_iterator<VALUE,RIGHT>>;
    
    template<typename LEFT , typename VALUE>
    using make_backward_iterator = mpl::function<mpl::backward_iterator<LEFT,VALUE>>;
    
    template<typename LEFT , typename VALUE , typename RIGHT>
    using make_bidirectional_iterator = mpl::function<mpl::bidirectional_iterator<LEFT,VALUE,RIGHT>>;
    


    template<typename T>
    struct begin_t;

    template<typename T>
    struct end_t;

    template<typename T>
    struct rbegin_t;

    template<typename T>
    struct rend_t;

    template<typename T>
    struct previous_t;

    template<typename T>
    struct next_t;
    
    template<typename T>
    using begin = typename mpl::begin_t<T>::result;
    
    template<typename T>
    using end = typename mpl::end_t<T>::result;
    
    template<typename T>
    using rbegin = typename mpl::rbegin_t<T>::result;
    
    template<typename T>
    using rend = typename mpl::rend_t<T>::result;
    
    template<typename T>
    using previous = typename mpl::previous_t<T>::result;
    
    template<typename T>
    using next = typename mpl::next_t<T>::result;

    template<typename T>
    using first = typename mpl::begin<T>::value;

    template<typename T>
    using last = typename mpl::rbegin<T>::value;
    
    

    template<typename VALUE1 , typename VALUE2>
    struct equal_t<mpl::iterator<VALUE1> , mpl::iterator<VALUE2>> : public mpl::function<mpl::equal<VALUE1 , VALUE2>> {};

    template<typename VALUE1 , typename RIGHT1 , typename VALUE2 , typename RIGHT2>
    struct equal_t<mpl::forward_iterator<VALUE1,RIGHT1> , forward_iterator<VALUE2,RIGHT2>> : public mpl::function<mpl::logical_and<equal<VALUE1,VALUE2> , mpl::equal<RIGHT1,RIGHT2>>> {};

    template<typename VALUE1 , typename LEFT1 , typename VALUE2 , typename LEFT2>
    struct equal_t<mpl::backward_iterator<LEFT1 , VALUE1> , backward_iterator<LEFT2,VALUE2>> : public mpl::function<mpl::logical_and<equal<VALUE1,VALUE2> , mpl::equal<LEFT1,LEFT2>>> {};

    template<typename LEFT1 , typename VALUE1 , typename RIGHT1 , typename LEFT2 , typename VALUE2 , typename RIGHT2>
    struct equal_t<mpl::bidirectional_iterator<LEFT1,VALUE1,RIGHT1> , mpl::bidirectional_iterator<LEFT2,VALUE2,RIGHT2>> : public mpl::function<decltype( mpl::equal<LEFT1,LEFT2>() && mpl::equal<VALUE1,VALUE2>() && mpl::equal<RIGHT1,RIGHT2>())> {};


    namespace
    {   
        template<typename ITERATOR , typename DISTANCE>
        struct _add_t;
        
        template<typename ITERATOR , std::size_t DISTANCE>
        struct _add_t<ITERATOR,mpl::size_t<DISTANCE>>
        {
            using result = typename _add_t<mpl::next<ITERATOR> , mpl::size_t<DISTANCE-1>>::result;
        };
        
        template<typename ITERATOR>
        struct _add_t<ITERATOR,mpl::size_t<0>>
        {
            using result = ITERATOR;
        };
        
        
        template<typename ITERATOR , typename DISTANCE>
        struct _sub_t;
        
        template<typename ITERATOR , std::size_t DISTANCE>
        struct _sub_t<ITERATOR,mpl::size_t<DISTANCE>>
        {
            using result = typename _sub_t<mpl::previous<ITERATOR> , mpl::size_t<DISTANCE-1>>::result;
        };
        
        template<typename ITERATOR>
        struct _sub_t<ITERATOR,mpl::size_t<0>>
        {
            using result = ITERATOR;
        };
    }
    
    template<typename VALUE , typename RIGHT , std::size_t DISTANCE>
    struct add_t<mpl::forward_iterator<VALUE,RIGHT>,mpl::size_t<DISTANCE>> : public mpl::function<typename _add_t<mpl::forward_iterator<VALUE,RIGHT>,mpl::size_t<DISTANCE>>::result> {};
    
    template<typename LEFT , typename VALUE , std::size_t DISTANCE>
    struct sub_t<mpl::backward_iterator<LEFT,VALUE>,mpl::size_t<DISTANCE>> : public mpl::function<typename _sub_t<mpl::backward_iterator<LEFT,VALUE>,mpl::size_t<DISTANCE>>::result> {};
    
    
}

#endif	/* ITERATOR_HPP */

