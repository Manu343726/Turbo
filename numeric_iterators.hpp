/* 
 * File:   numeric_iterators.hpp
 * Author: paradise
 *
 * Created on 4 de septiembre de 2013, 15:37
 */

#ifndef NUMERIC_ITERATORS_HPP
#define	NUMERIC_ITERATORS_HPP

#include "iterator.hpp"
#include "basic_types.hpp"
#include "fixed_point.hpp"
#include "operators.hpp"
#include "for_loops.hpp"

#include <limits>

namespace mpl
{
    template<typename T , T v>
    struct begin_t<mpl::value_t<T,v>> : mpl::make_forward_iterator<mpl::value_t<T, std::numeric_limits<T>::min()> , mpl::no_type> {};
    
    template<typename T , T v>
    struct end_t<mpl::value_t<T,v>> : mpl::make_forward_iterator<mpl::value_t<T, std::numeric_limits<T>::max()> , mpl::no_type> {};
    
    template<typename T , T v>
    struct rbegin_t<mpl::value_t<T,v>> : mpl::make_backward_iterator<mpl::no_type,mpl::value_t<T, std::numeric_limits<T>::min()>> {};
    
    template<typename T , T v>
    struct rend_t<mpl::value_t<T,v>> : mpl::make_backward_iterator<mpl::no_type,mpl::value_t<T, std::numeric_limits<T>::max()>> {};
    
    
    template<typename T , T v>
    struct previous_t<mpl::backward_iterator<mpl::no_type,mpl::value_t<T,v>>> : mpl::make_backward_iterator<mpl::no_type,mpl::value_t<T,v-1>> {};
    
    template<typename T , T v>
    struct next_t<mpl::forward_iterator<mpl::value_t<T,v>,mpl::no_type>> : mpl::make_forward_iterator<mpl::value_t<T,v+1>,mpl::no_type> {};
    
    
    namespace
    {
#define MAKE_NUMERIC_INTEGRAL_ITERATOR(type_name) template<mpl::type_name##_type VALUE> \
                                         using make_##type_name##_forward_iterator = mpl::forward_iterator<mpl::type_name<VALUE>,mpl::no_type>; \
                                         template<mpl::type_name##_type VALUE> \
                                         using make_##type_name##_backward_iterator = mpl::backward_iterator<mpl::no_type,mpl::type_name<VALUE>>
    }
        
    MAKE_NUMERIC_INTEGRAL_ITERATOR(ucharacter);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(character);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(uinteger);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(integer);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(ulong_integer);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(long_integer);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(ulong_long_integer);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(long_long_integer);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(boolean);
    MAKE_NUMERIC_INTEGRAL_ITERATOR(size_t );
    
}

#endif	/* NUMERIC_ITERATORS_HPP */

