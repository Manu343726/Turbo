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

#ifndef NUMERIC_ITERATORS_HPP
#define	NUMERIC_ITERATORS_HPP

#include "iterator.hpp"
#include "core.hpp"
#include "fixed_point.hpp"
#include "for_loops.hpp"

#include <limits>

namespace tb
{
    template<typename T , T v>
    struct begin_t<tb::value_t<T,v>> : tb::make_forward_iterator<tb::value_t<T, std::numeric_limits<T>::min()> , tb::no_type> {};
    
    template<typename T , T v>
    struct end_t<tb::value_t<T,v>> : tb::make_forward_iterator<tb::value_t<T, std::numeric_limits<T>::max()> , tb::no_type> {};
    
    template<typename T , T v>
    struct rbegin_t<tb::value_t<T,v>> : tb::make_backward_iterator<tb::no_type,tb::value_t<T, std::numeric_limits<T>::min()>> {};
    
    template<typename T , T v>
    struct rend_t<tb::value_t<T,v>> : tb::make_backward_iterator<tb::no_type,tb::value_t<T, std::numeric_limits<T>::max()>> {};
    
    
    
    template<typename T , T v , typename STEP>
    struct next_t<tb::forward_iterator<tb::value_t<T,v>,tb::no_type>,STEP> : tb::make_forward_iterator<tb::value_t<T,v+STEP::value>,tb::no_type> {};
    
    template<typename T , T v>
    struct next_t<tb::forward_iterator<tb::value_t<T,v>,tb::no_type>,tb::no_type> : tb::make_forward_iterator<tb::value_t<T,v+1>,tb::no_type> {};
    
    template<typename T , T v , typename STEP>
    struct previous_t<tb::backward_iterator<tb::value_t<T,v>,tb::no_type>,STEP> : tb::make_backward_iterator<tb::no_type,tb::value_t<T,v-STEP::value>> {};
    
    template<typename T , T v>
    struct previous_t<tb::backward_iterator<tb::value_t<T,v>,tb::no_type>,tb::no_type> : tb::make_backward_iterator<tb::no_type , tb::value_t<T,v-1>> {};
    
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct begin_t<tb::fixed_point<BITS,PRECISION>> : tb::make_forward_iterator<tb::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , tb::no_type> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct end_t<tb::fixed_point<BITS,PRECISION>> : tb::make_forward_iterator<tb::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , tb::no_type> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct rbegin_t<tb::fixed_point<BITS,PRECISION>> : tb::make_backward_iterator<tb::no_type,tb::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct rend_t<tb::fixed_point<BITS,PRECISION>> : tb::make_backward_iterator<tb::no_type,tb::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION , typename STEP>
    struct next_t<tb::forward_iterator<tb::fixed_point<BITS,PRECISION>,tb::no_type>,STEP> : tb::make_forward_iterator<tb::add<tb::fixed_point<BITS,PRECISION>,STEP>,tb::no_type> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct next_t<tb::forward_iterator<tb::fixed_point<BITS,PRECISION>,tb::no_type>,tb::no_type> : tb::make_forward_iterator<tb::add<tb::fixed_point<BITS,PRECISION>,tb::one<tb::fixed_point<BITS,PRECISION>>>,tb::no_type> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION , typename STEP>
    struct previous_t<tb::backward_iterator<tb::no_type,tb::fixed_point<BITS,PRECISION>>,STEP> : tb::make_backward_iterator<tb::no_type , tb::sub<tb::fixed_point<BITS,PRECISION>,STEP>> {};
    
    template<tb::fixed_point_bits BITS , tb::fixed_point_precision PRECISION>
    struct previous_t<tb::backward_iterator<tb::no_type,tb::fixed_point<BITS,PRECISION>>,tb::no_type> : tb::make_backward_iterator<tb::no_type , tb::sub<tb::fixed_point<BITS,PRECISION>,tb::one<tb::fixed_point<BITS,PRECISION>>>> {};
    
    
    namespace
    {
#define MAKE_NUMERIC_INTEGRAL_ITERATOR(type_name) template<tb::type_name##_type VALUE> \
                                         using make_##type_name##_forward_iterator = tb::forward_iterator<tb::type_name<VALUE>,tb::no_type>; \
                                         template<tb::type_name##_type VALUE> \
                                         using make_##type_name##_backward_iterator = tb::backward_iterator<tb::no_type,tb::type_name<VALUE>>
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
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = tb::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_forward_iterator = tb::forward_iterator<tb::decimal<mantissa,exponent,PRECISION>,tb::no_type>;
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = tb::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_backword_iterator = tb::backward_iterator<tb::no_type,tb::decimal<mantissa,exponent,PRECISION>>;
}

#endif	/* NUMERIC_ITERATORS_HPP */

