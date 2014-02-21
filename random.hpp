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

#ifndef RANDOM_HPP
#define	RANDOM_HPP

#include "core/basic_types.hpp"
#include "numeric_iterators.hpp"


namespace tml
{
    using default_random_seed = tml::uinteger<__LINE__>;
    
    using lcg_a = tml::uinteger<-1>;
    using lcg_c = tml::uinteger<0>;
    using lcg_m = tml::uinteger<48271>;
    
    
    template<typename SEED>
    using random = tml::uinteger<( tml::lcg_a::value * SEED::value + tml::lcg_c::value ) % tml::lcg_m::value>;
    
    
    TURBO_DEFINE_FUNCTION( generate_n_random , (typename SEED , typename COUNT) , (SEED , COUNT) );
    
    template<tml::uinteger_type seed , tml::size_t_type count>
    struct generate_n_random_t<tml::uinteger<seed>,tml::size_t<count>>
    {
        template<typename CURRENT , typename NUMBERS>
        struct kernel;
        
        template<typename... NUMBERS , typename LAST , tml::size_t_type CURRENT>
        struct kernel<tml::size_t<CURRENT>,tml::list<NUMBERS...,LAST>>
        {
            using current_seed = LAST;
            using number = tml::random<current_seed>;
            
            using result = tml::list<NUMBERS...,number>;
        };
        
        using result = tml::for_loop<tml::make_size_t_forward_iterator<0>,
                                     tml::make_size_t_forward_iterator<count>,
                                     tml::list<tml::uinteger<seed>>,
                                     kernel
                                    >;
    };
}

#endif	/* RANDOM_HPP */

