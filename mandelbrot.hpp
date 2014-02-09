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

#ifndef MANDELBROT_HPP
#define	MANDELBROT_HPP

#include "iterator.hpp"
#include "for_loops.hpp"
#include "complex.hpp"
#include "color.hpp"

#include <iostream>
#include <fstream>
#include <iterator>


namespace mandelbrot
{
    const int begin_point = -1;
    const int end_point   =  1;

    using begin = tml::make_decimal_forward_iterator<begin_point>;
    using end   = tml::make_decimal_forward_iterator<end_point>;
    using size  = tml::uinteger<4>; //Size in tiles 
    using step  = decltype( (tml::decimal<end_point>() - tml::decimal<begin_point>()) / size() );

    using convergence_value = tml::decimal<1>;
    using iteration_begin   = tml::make_uinteger_forward_iterator<0>;
    using iteration_end     = tml::make_uinteger_forward_iterator<10>;

    template<typename Y>
    struct outer_loop
    {
        template<typename X>
        struct inner_loop
        {
            using number = math::complex<X,Y>;

            template<typename INDEX , typename PREVIOUS>
            struct iterate_number
            {
                using result = decltype( PREVIOUS() * PREVIOUS() + tml::one<PREVIOUS>() );

                using abort = decltype( math::square_length<result>() > convergence_value );
            };

            using result_number = tml::for_loop<iteration_begin , iteration_end , number , iterate_number , step>;
            //using result_number = number;

            using result = tml::conditional<tml::true_type , gfx::from_rgb<0,0,0> , gfx::from_rgb<255,255,255>>;
            //using result = gfx::from_rgb<255,255,255>;
        };

        using result = tml::for_each<begin,end,inner_loop,tml::true_predicate,step>;
    };
    
    using execute = tml::for_each<begin,end,outer_loop,tml::true_predicate,step>;
    
    
    template<typename LIST>
    struct to_array;

    template<typename... Ts>
    struct to_array<tml::list<Ts...>>
    {
        static constexpr unsigned int result[] = { Ts::value... };
    }; 

    template<typename LIST>
    struct to_2d_array;

    template<typename... Ts>
    struct to_2d_array<tml::list<Ts...>>
    {
        static constexpr unsigned int result[sizeof...(Ts)][sizeof...(Ts)] = { to_array<Ts>::result... };
    };
    
    template<typename LIST>
    void dump_to_file()
    {
        auto begin = std::begin( to_2d_array<LIST>::result );
        auto end   = std::end( to_2d_array<LIST>::result );
        
        std::ofstream os("mandelbrot_output.ppm");
        
        if(os)
        {
            os << "P6 " << size::value << " " << size::value << "255 ";
            
            std::copy(begin,end,std::ostream_iterator<unsigned int>(os));
        }   
    }
}

#endif	/* MANDELBROT_HPP */

