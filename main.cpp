/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

//#define MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION 8

#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "for_loops.hpp"
#include "numeric_iterators.hpp"
#include "sort.hpp"
#include "fixed_point.hpp"
#include "vector.hpp"
#include "trigonometry.hpp"
#include "matrix3x3.hpp"
#include "matrix4x4.hpp"
#include "prime_filter.hpp"
#include "color.hpp"
#include "sqrt.hpp"
#include "mandelbrot.hpp"
#include "complex.hpp"

#include <iostream>

using a = math::complex<mpl::decimal<1>,mpl::decimal<0>>;
using b = math::complex<mpl::decimal<0>,mpl::decimal<1>>;
using c = decltype( a() * b() + b() / a() );

using mandelbrot_image = mandelbrot::execute;

int main()
{
    std::cout << mpl::to_string<c>() << std::endl;
    std::cout << mpl::to_string<mpl::conditional<decltype(mpl::uinteger<0>() > mpl::uinteger<1>()),a,b>>() << std::endl;
    
    mandelbrot::dump_to_file<mandelbrot_image>();
}

