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

namespace tml
{
    template<typename T , T v>
    struct begin_t<tml::value_t<T,v>> : tml::make_forward_iterator<tml::value_t<T, std::numeric_limits<T>::min()> , tml::no_type> {};
    
    template<typename T , T v>
    struct end_t<tml::value_t<T,v>> : tml::make_forward_iterator<tml::value_t<T, std::numeric_limits<T>::max()> , tml::no_type> {};
    
    template<typename T , T v>
    struct rbegin_t<tml::value_t<T,v>> : tml::make_backward_iterator<tml::no_type,tml::value_t<T, std::numeric_limits<T>::min()>> {};
    
    template<typename T , T v>
    struct rend_t<tml::value_t<T,v>> : tml::make_backward_iterator<tml::no_type,tml::value_t<T, std::numeric_limits<T>::max()>> {};
    
    
    
    template<typename T , T v , typename STEP>
    struct next_t<tml::forward_iterator<tml::value_t<T,v>,tml::no_type>,STEP> : tml::make_forward_iterator<tml::value_t<T,v+STEP::value>,tml::no_type> {};
    
    template<typename T , T v>
    struct next_t<tml::forward_iterator<tml::value_t<T,v>,tml::no_type>,tml::no_type> : tml::make_forward_iterator<tml::value_t<T,v+1>,tml::no_type> {};
    
    template<typename T , T v , typename STEP>
    struct previous_t<tml::backward_iterator<tml::value_t<T,v>,tml::no_type>,STEP> : tml::make_backward_iterator<tml::no_type,tml::value_t<T,v-STEP::value>> {};
    
    template<typename T , T v>
    struct previous_t<tml::backward_iterator<tml::value_t<T,v>,tml::no_type>,tml::no_type> : tml::make_backward_iterator<tml::no_type , tml::value_t<T,v-1>> {};
    
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct begin_t<tml::fixed_point<BITS,PRECISION>> : tml::make_forward_iterator<tml::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , tml::no_type> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct end_t<tml::fixed_point<BITS,PRECISION>> : tml::make_forward_iterator<tml::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , tml::no_type> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct rbegin_t<tml::fixed_point<BITS,PRECISION>> : tml::make_backward_iterator<tml::no_type,tml::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct rend_t<tml::fixed_point<BITS,PRECISION>> : tml::make_backward_iterator<tml::no_type,tml::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION , typename STEP>
    struct next_t<tml::forward_iterator<tml::fixed_point<BITS,PRECISION>,tml::no_type>,STEP> : tml::make_forward_iterator<tml::add<tml::fixed_point<BITS,PRECISION>,STEP>,tml::no_type> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct next_t<tml::forward_iterator<tml::fixed_point<BITS,PRECISION>,tml::no_type>,tml::no_type> : tml::make_forward_iterator<tml::add<tml::fixed_point<BITS,PRECISION>,tml::one<tml::fixed_point<BITS,PRECISION>>>,tml::no_type> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION , typename STEP>
    struct previous_t<tml::backward_iterator<tml::no_type,tml::fixed_point<BITS,PRECISION>>,STEP> : tml::make_backward_iterator<tml::no_type , tml::sub<tml::fixed_point<BITS,PRECISION>,STEP>> {};
    
    template<tml::fixed_point_bits BITS , tml::fixed_point_precision PRECISION>
    struct previous_t<tml::backward_iterator<tml::no_type,tml::fixed_point<BITS,PRECISION>>,tml::no_type> : tml::make_backward_iterator<tml::no_type , tml::sub<tml::fixed_point<BITS,PRECISION>,tml::one<tml::fixed_point<BITS,PRECISION>>>> {};
    
    
    namespace
    {
#define MAKE_NUMERIC_INTEGRAL_ITERATOR(type_name) template<tml::type_name##_type VALUE> \
                                         using make_##type_name##_forward_iterator = tml::forward_iterator<tml::type_name<VALUE>,tml::no_type>; \
                                         template<tml::type_name##_type VALUE> \
                                         using make_##type_name##_backward_iterator = tml::backward_iterator<tml::no_type,tml::type_name<VALUE>>
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
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = tml::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_forward_iterator = tml::forward_iterator<tml::decimal<mantissa,exponent,PRECISION>,tml::no_type>;
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = tml::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_backword_iterator = tml::backward_iterator<tml::no_type,tml::decimal<mantissa,exponent,PRECISION>>;
}

#endif	/* NUMERIC_ITERATORS_HPP */

