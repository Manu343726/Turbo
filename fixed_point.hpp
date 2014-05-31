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

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include <cstdint>

#include "algebra.hpp"
#include "function.hpp"
#include "utility.hpp"

namespace tml
{
    namespace impl
    {
        /*
         * Underlying integer type of tml::fsingle single precision fixed-point type 
         */
        using single_t = std::int32_t;
        
        constexpr const single_decimal_bits = tml::util::sizeof_bits<tml::impl::single_t>::value / 4;
        
        /*
         * Underlying integer type of tml::fdouble double precision fixed-point type 
         */
        using double_t = std::int64_t;
        
        constexpr const double_decimal_bits = tml::util::sizeof_bits<tml::impl::single_t>::value / 4;
    }
    
    template<typename INTEGER_T , INTEGER_T N>
    struct fixed_point
    {};
    
    template<tml::impl::single_t N>
    using fsingle = tml::fixed_point<tml::impl::single_t,(N << tml::util::sizeof_bits<tml::impl::single_t>::value)>;
    
    template<tml::impl::double_t N>
    using fdouble = tml::fixed_point<tml::impl::double_t,(N << tml::util::sizeof_bits<tml::impl::double_t>::value)>;
    
    
    namespace impl
    {
        template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
        struct add<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
            public tml::function<tml::fixed_point<INTEGER_T,LHS + RHS>
        {};
        
        template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
        struct sub<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
            public tml::function<tml::fixed_point<INTEGER_T,LHS - RHS>
        {};
        
        template<typename INTEGER_T , INTEGER_T LHS , INTEGER_T RHS>
        struct mul<tml::fixed_point<INTEGER_T,LHS>,tml::fixed_point<INTEGER_T,RHS>> :
        public tml::function<tml::fixed_point<INTEGER_T,((LHS * RHS) >> tml::util::sizeof_bits<INTEGER_T>::value)>
        {};
    }
}

#endif	/* FIXED_POINT_HPP */

