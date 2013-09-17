Turbo
=====

*C++11 metaprogramming library*


# What is Turbo?

Turbo is a library dessigned to provide compile-time utilities through [template metaprogramming](http://en.wikipedia.org/wiki/Template_metaprogramming).





# Features

## Portable `to_string()` function to print type names at runtime:  

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

## Compile-time basic types wrappers: Compile-time arithmetic

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

### Compile-time arithmetic:
The library provides a set of default metafunctions to perform **arithmetic, bitshift, logical, and comparison operations with the basic type wrappers**. Type casting is correctly performed in that computations to avoid signed-unsigned overflows and other related problems:

    #include "operators.hpp"
    #include "basic_types.hpp"
    
    int main()
    {
    	using a = mpl::uinteger<1>;
    	using b = mpl::integer<2>;
    	
    	using c = mpl::add<a,b>; //c is mpl::integer<3>
    }

### Expression templates: Operators overloading for a simple and more readable expression syntax

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




