/* 
 * File:   fixed_point.hpp
 * Author: paradise
 *
 * Created on 5 de septiembre de 2013, 13:31
 */

#ifndef FIXED_POINT_HPP
#define	FIXED_POINT_HPP

#include "basic_types.hpp"
#include "operators.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <cmath> //std::pow())


namespace mpl
{
    template<int base , int exponent>
    struct positive_pow : public mpl::long_long_integer<base * positive_pow<base,exponent-1>::value> {};

    template<int base>
    struct positive_pow<base,0> : public mpl::long_long_integer<1> {};

    template<long long int number , int shift>
    struct decimal_leftshift : public mpl::long_long_integer<number * positive_pow<10, shift>::value> {};

    template<long long int number , int shift>
    struct decimal_rightshift : public mpl::long_long_integer<number / positive_pow<10, shift>::value> {};

    template<bool CONDITION , int NUMBER , int SHIFT>
    struct decimal_shift_chooser
    {
        using shifter = decimal_leftshift<NUMBER,SHIFT>;
    };

    template<long long NUMBER , int SHIFT>
    struct decimal_shift_chooser<false,NUMBER,SHIFT>
    {
        using shifter = decimal_rightshift<NUMBER,-SHIFT>;
    };

    template<int number , int shift>
    struct decimal_shift
    {
        using shifter = typename decimal_shift_chooser<( shift >= 0 ) , number , shift>::shifter;
        static const long long int value = shifter::value;
    };

    /* Fixed-point implementation: */

    using fixed_point_bits      = long long int;
    using fixed_point_precision = unsigned int;

#ifndef MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = 8; 
#else
    const fixed_point_precision DEFAULT_FRACTIONAL_PRECISION = MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION; 
#endif

    template<fixed_point_bits BITS , fixed_point_precision PRECISION = DEFAULT_FRACTIONAL_PRECISION>
    struct fixed_point
    {
        operator float()
        {
            return (float)BITS * std::pow(10.0f,-(float)PRECISION);
        };
    };

    template<int mantissa , int exponent = 0 , fixed_point_precision PRECISION = mpl::DEFAULT_FRACTIONAL_PRECISION> // MANTISSA x 10^EXPONENT
    using decimal = mpl::fixed_point<decimal_shift<mantissa , PRECISION + exponent>::value , PRECISION>; 


    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct zero_t<mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::fixed_point<0,PRECISION>> {};

    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct one_t<mpl::fixed_point<BITS,PRECISION>> : public mpl::function<fixed_point<decimal_leftshift<1,PRECISION>::value,PRECISION>> {};

    
    /* Comparison operator */
    
    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision P1 , fixed_point_precision P2>
    struct equal_t<mpl::fixed_point<BITS1,P1> , mpl::fixed_point<BITS2,P2>> : public mpl::function<mpl::boolean<BITS1 == BITS2 && P1 == P2>> {};

    /* Arithmetic operators */

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct add_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<fixed_point<BITS1+BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct sub_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<fixed_point<BITS1-BITS2 , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct mul_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<fixed_point<decimal_rightshift<BITS1*BITS2,PRECISION>::value , PRECISION>> {};

    template<fixed_point_bits BITS1 , fixed_point_bits BITS2 , fixed_point_precision PRECISION>
    struct div_t<mpl::fixed_point<BITS1,PRECISION> , mpl::fixed_point<BITS2,PRECISION>> : public mpl::function<fixed_point<decimal_leftshift<BITS1,PRECISION>::value/BITS2 , PRECISION>> {};
    
    
    /* Operations between integral and fixed-point values */
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::add<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct add_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::add<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::sub<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct sub_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::sub<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::mul<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct mul_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::mul<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};
    
    
    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<mpl::value_t<T,v>,mpl::fixed_point<BITS,PRECISION>> : public mpl::function<mpl::div<mpl::decimal<(fixed_point_bits)v,0,PRECISION>,mpl::fixed_point<BITS,PRECISION>>> {};

    template<typename T , T v , fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct div_t<mpl::fixed_point<BITS,PRECISION>,mpl::value_t<T,v>> : public mpl::function<mpl::div<mpl::fixed_point<BITS,PRECISION>,mpl::decimal<(fixed_point_bits)v,0,PRECISION>>> {};




    /* mpl::to_string */
    
    template<fixed_point_bits BITS , fixed_point_precision PRECISION>
    struct to_string_t<mpl::fixed_point<BITS,PRECISION>>
    {
        operator std::string() const
        {
            std::ostringstream os;
            os << mpl::fixed_point<BITS,PRECISION>();
            return os.str();
        }
    };
}

#endif	/* FIXED_POINT_HPP */

