Turbo
=====

*C++11 metaprogramming library*

## What is Turbo?

Turbo is a library dessigned to provide compile-time utilities through [template metaprogramming](http://en.wikipedia.org/wiki/Template_metaprogramming).

## Features

Turbo provides a set of features to simplify type manipulation and compile-time computations:

### Portable `to_string()` function to print type names at runtime:  

Turbo implements demangling for the most common compilers<sup>1</sup> to provide a function which returns the `std::string` representation of a type. For example:

    #include "to_string.hpp" 
    #include <iostream>
    #include <vector>

    int main()
    {
        std::cout << mpl::to_string<std::vector<int>>() << std::endl;
    }

Output: 
> `vector<int>`
   
 
   *[1] GCC and MSVC only demangling implementation yet.*

### Compile-time basic types wrappers: Compile-time arithmetic

Turbo provides a set of predefined templates which are equivalent to the most common C++ basic types, such as `bool`, `char`, `integer`, etc: `mpl::boolean`, 
`mpl::character`, `mpl::integer`.   

The function `mpl::to_string()` is specialized to provide a natural and readable string value of the wrappers. For example:

    #include "basic_types.hpp"
    #include "to_string.hpp"
    
    #include <iostream>
    
    int main()
    {
    	using bool_value = mpl::boolean<true>;
    	using char_value = mpl::character<'a'>;
    	using integer_value = mpl::integer<10>;
    	
    	std::cout << mpl::to_string<bool_value>() << " ";
    	std::cout << mpl::to_string<char_value>() << " ";
    	std::cout << mpl::to_string<integer_value>();
    }

Output:
> true a 10

#### Compile-time arithmetic:
The library provides a set of default metafunctions to perform **arithmetic, bitshift, logical, and comparison operations with the basic type wrappers**. Type casting is correctly performed in that computations to avoid signed-unsigned overflows and other related problems:

    #include "operators.hpp"
    #include "basic_types.hpp"
    
    int main()
    {
    	using a = mpl::uinteger<1>;
    	using b = mpl::integer<2>;
    	
    	using c = mpl::add<a,b>; //c is mpl::integer<3>
    }

#### Expression templates: Operators overloading for a simple and more readable expression syntax

Turbo overloads the most common operators to implement expression templates and provide, within type inspection through the [`decltype` specifier](http://en.cppreference.com/w/cpp/language/decltype), the functions described above in a more confortable syntax. This is specially usefull when the expressions are complex. For example:

    #include "operators.hpp"
    #include "basic_types.hpp"
    #include "expressions.hpp"
    
    int main()
    {
    	using x = mpl::integer<1>;
    	using y = mpl::integer<2>;
    	using z = mpl::integer<3>;
    	
    	//The following expression is equivalent to
    	//mpl::add<mpl::add<mpl::mul<x,x>,mpl::mul<y,y>>,mpl::mul<z,z>>;:
    
    	using square_length = decltype( x()*x() + y()*y() + z()*z() );
    }

*NOTE: This is an example of the computation of the length of a 3d vector at compile-time. The example computes the square-length of the vector, because the `square_root` function still is not implemented.*

### Compile-time fixed-point arithmetic

Turbo implements decimal fixed-point arithmetic through the `mpl::fixed_point` type. This type provides compile-time fixed-point arithmetic through specializations of the arithmetic metafunctions described above.  

`mpl::fixed_point` implements decimal fixed-point numbers with the specified precision (The number of digits which represent the fractional part of the number):

    template<mpl::fixed_point_bits BITS , mpl::fixed_point_precission PRECISION>
    struct fixed_point {};
 
 By default `mpl::fixed_point_bits` is a `long long int`, that is, is 64 bits length. So `mpl::fixed_point` **could represent decimal numbers up to 19 digits**. Of course that 19 digits include the fractional digits. By default Turbo sets the precision of `mpl::fixed_point` at 8 digits.
 
 The way to instantiate fixed point numbers is tu cumbersome, because you have to take into account the precision of the number and provide the full integer number. For example:
 
    using pi = mpl::fixed_point<314150000,8>; //3,1415 with 8 digits precision 

To deal with that problem, the library provides the alias `mpl::decimal`, which implements **decimal scientific notation**:

    using pi = mpl::decimal<31415,-4>; //31415 x 10^-4 (3,1415)
 
 Of course `mpl::fixed_point` specializes `mpl::to_string` :
 
     #include "fixed_point.hpp"
     #include "expressions.hpp"
     #include "to_string.hpp"
     
     using pi      = mpl::decimal<314159265,-8>; //3,14159265
     using radious = mpl::decimal<10>; //10
     using circle_length = decltype( mpl::decimal<2>() * pi() * radious() );
     
     int main()
     {
     	std::cout << "Radious: " << mpl::to_string<radious>() << std::endl;
     	std::cout << "Circle length: " << mpl::to_string<circle_length>() << std::endl;
     }
> Radious: 10  
Circle length: 62,831853

#### Trigonometric functions
In addition to the arithmetic operations shared with the integral values, Turbo implements compile-time trigonometric functions such as sine and cosine through Taylor-McLaurin series:

    #include "trigonometry.hpp"
    #include "fixed_point.hpp"
    #include "to_string.hpp"
    
    using deg_0 = mpl::decimal<0>;
    using deg_45 = decltype( math::pi() / mpl::decimal<4>() );
    using deg_90 = decltype( math::pi() / mpl::decimal<2>() );
    
    int main()
    {
    	std::cout << "sin(0º) = "  << mpl::to_string<math::sin<deg_0>>() << std::endl;
    	std::cout << "sin(45º) = " << mpl::to_string<math::sin<deg_45>>() << std::endl;
    	std::cout << "sin(90º) = " << mpl::to_string<math::sin<deg_90>>() << std::endl;
    	
    	std::cout << "cos(0º) = "  << mpl::to_string<math::cos<deg_0>>() << std::endl;
    	std::cout << "cos(45º) = " << mpl::to_string<math::cos<deg_45>>() << std::endl;
    	std::cout << "cos(90º) = " << mpl::to_string<math::cos<deg_90>>() << std::endl;
    }
> sin(0º) = 0  
sin(45º) = 0,707107   
sin(90º) = 1  
cos(0º) = 1  
cos(45º) = 0,07107  
cos(90º) = 1e-08

As the example shows, the implementation has little precision errors (`cos(90º)` should be zero). Turbo uses a MacLaurin series aproximation of 10 terms by default, but the arithmetric functions are parametrized to allow specifying the number of terms used in the aproximations:

    using cos_deg_90 = math::cos<deg_90,mpl::uinteger<200>>; //cos(90º) computed using 200 terms. 

