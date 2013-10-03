/* 
 * File:   mandelbrot.hpp
 * Author: manu343726
 *
 * Created on 29 de septiembre de 2013, 23:59
 */

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

    using begin = mpl::make_decimal_forward_iterator<begin_point>;
    using end   = mpl::make_decimal_forward_iterator<end_point>;
    using size  = mpl::uinteger<1>; //Size in tiles 
    using step  = decltype( (mpl::decimal<end_point>() - mpl::decimal<begin_point>()) / size() );

    using convergence_value = mpl::decimal<1>;
    using iteration_begin   = mpl::make_uinteger_forward_iterator<0>;
    using iteration_end     = mpl::make_uinteger_forward_iterator<10>;

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
                using result = decltype( PREVIOUS() * PREVIOUS() + mpl::one<PREVIOUS>() );

                static const bool abort = decltype( math::square_length<result>() > convergence_value )::value;
            };

            //using result_number = mpl::for_loop<iteration_begin , iteration_end , number , iterate_number , step>; COMPILER SEGFAULT HERE
            using result_number = number;

            using result = mpl::conditional<mpl::true_type , gfx::from_rgb<0,0,0> , gfx::from_rgb<255,255,255>>;
            //using result = gfx::from_rgb<255,255,255>;
        };

        using result = mpl::for_each<begin,end,inner_loop,mpl::true_predicate,step>;
    };
    
    using execute = mpl::for_each<begin,end,outer_loop,mpl::true_predicate,step>;
    
    
    template<typename LIST>
    struct to_array;
    
    template<typename... Ts>
    struct to_array<mpl::list<Ts...>>
    {      
        template<typename list>
        struct dump_sub_array;
        
        template<typename... Us>
        struct dump_sub_array<mpl::list<Us...>>
        {
            static constexpr unsigned int result[size::value] = { Us::value... };
        };
        
        static constexpr unsigned int result[size::value][size::value] = { {dump_sub_array<Ts>::result...} };
    };
    
    template<typename LIST>
    void dump_to_file()
    {
        auto begin = std::begin( to_array<LIST>::result );
        auto end   = std::end( to_array<LIST>::result );
        
        std::ofstream os("mandelbrot_output.ppm");
        
        if(os)
        {
            os << "P6 " << size::value << " " << size::value << "255 ";
            
            std::copy(begin,end,std::ostream_iterator<unsigned int>(os));
        }   
    }
}

#endif	/* MANDELBROT_HPP */

