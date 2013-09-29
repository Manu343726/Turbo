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
#include <iterator>


namespace mandelbrot
{
    const int begin_point = -1;
    const int end_point   =  1;

    using begin = mpl::make_decimal_forward_iterator<begin_point>;
    using end   = mpl::make_decimal_forward_iterator<end_point>;
    using size  = mpl::uinteger<10>; //Size in tiles 
    using step  = decltype( (mpl::decimal<begin_point>() - mpl::decimal<end_point>()) / size() );

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

            using result_number = mpl::for_loop<iteration_begin , iteration_end , number , iterate_number , step>; 

            using result = mpl::conditional<decltype( (math::square_length<result_number>() > convergence_value() ) ) , gfx::from_rgb<0,0,0> , gfx::from_rgb<255,255,255>>;
        };

        using result = mpl::for_each<begin,end,inner_loop,mpl::true_predicate,step>;
    };
    
    using execute = mpl::for_each<begin,end,outer_loop,mpl::true_predicate,step>;
    
    
    template<typename LIST>
    struct stract_to_array;
    
    template<typename... Ts>
    struct stract_to_array<mpl::list<Ts...>>
    {
        template<typename LIST>
        struct stract_sub_array;
        
        template<typename... Us>
        struct stract_sub_array
        {
            static const unsigned int result[] = { Us()... };
            using array_type = unsigned int[sizeof...(Us)];
        };
        
        static const typename stract_sub_array<mpl::first<mpl::list<Ts...>>>::array_type result[] = { stract_sub_array<Ts>::result... };
    };
    
    template<typename LIST>
    void dump_to_file()
    {
        auto begin = std::begin( stract_to_array<LIST>::result );
        auto end   = std::end( stract_to_array<LIST>::result );
        
        std::ofstream os("raytracer_output.ppm");
        
        if(os)
        {
            os << "P6 " << size::value << " " << size::value << "255 ";
            
            std::copy(begin,end,std::ostream_iterator(os));
        }   
    }
}

#endif	/* MANDELBROT_HPP */

