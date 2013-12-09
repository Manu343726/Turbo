/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013, Manuel Sánchez Pérez                                    *
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
    
    
    
    template<typename T , T v , typename STEP>
    struct next_t<mpl::forward_iterator<mpl::value_t<T,v>,mpl::no_type>,STEP> : mpl::make_forward_iterator<mpl::value_t<T,v+STEP::value>,mpl::no_type> {};
    
    template<typename T , T v>
    struct next_t<mpl::forward_iterator<mpl::value_t<T,v>,mpl::no_type>,mpl::no_type> : mpl::make_forward_iterator<mpl::value_t<T,v+1>,mpl::no_type> {};
    
    template<typename T , T v , typename STEP>
    struct previous_t<mpl::backward_iterator<mpl::value_t<T,v>,mpl::no_type>,STEP> : mpl::make_backward_iterator<mpl::no_type,mpl::value_t<T,v-STEP::value>> {};
    
    template<typename T , T v>
    struct previous_t<mpl::backward_iterator<mpl::value_t<T,v>,mpl::no_type>,mpl::no_type> : mpl::make_backward_iterator<mpl::no_type , mpl::value_t<T,v-1>> {};
    
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct begin_t<mpl::fixed_point<BITS,PRECISION>> : mpl::make_forward_iterator<mpl::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , mpl::no_type> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct end_t<mpl::fixed_point<BITS,PRECISION>> : mpl::make_forward_iterator<mpl::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION> , mpl::no_type> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct rbegin_t<mpl::fixed_point<BITS,PRECISION>> : mpl::make_backward_iterator<mpl::no_type,mpl::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct rend_t<mpl::fixed_point<BITS,PRECISION>> : mpl::make_backward_iterator<mpl::no_type,mpl::fixed_point<std::numeric_limits<fixed_point_bits>::min(),PRECISION>> {};
    
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION , typename STEP>
    struct next_t<mpl::forward_iterator<mpl::fixed_point<BITS,PRECISION>,mpl::no_type>,STEP> : mpl::make_forward_iterator<mpl::add<mpl::fixed_point<BITS,PRECISION>,STEP>,mpl::no_type> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct next_t<mpl::forward_iterator<mpl::fixed_point<BITS,PRECISION>,mpl::no_type>,mpl::no_type> : mpl::make_forward_iterator<mpl::add<mpl::fixed_point<BITS,PRECISION>,mpl::one<mpl::fixed_point<BITS,PRECISION>>>,mpl::no_type> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION , typename STEP>
    struct previous_t<mpl::backward_iterator<mpl::no_type,mpl::fixed_point<BITS,PRECISION>>,STEP> : mpl::make_backward_iterator<mpl::no_type , mpl::sub<mpl::fixed_point<BITS,PRECISION>,STEP>> {};
    
    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precision PRECISION>
    struct previous_t<mpl::backward_iterator<mpl::no_type,mpl::fixed_point<BITS,PRECISION>>,mpl::no_type> : mpl::make_backward_iterator<mpl::no_type , mpl::sub<mpl::fixed_point<BITS,PRECISION>,mpl::one<mpl::fixed_point<BITS,PRECISION>>>> {};
    
    
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
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = mpl::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_forward_iterator = mpl::forward_iterator<mpl::decimal<mantissa,exponent,PRECISION>,mpl::no_type>;
    
    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = mpl::DEFAULT_FRACTIONAL_PRECISION>
    using make_decimal_backword_iterator = mpl::backward_iterator<mpl::no_type,mpl::decimal<mantissa,exponent,PRECISION>>;
}

#endif	/* NUMERIC_ITERATORS_HPP */

