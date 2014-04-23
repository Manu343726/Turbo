Turbo
=====

*C++11 metaprogramming library*

## What is Turbo?

Turbo is a library dessigned to provide compile-time utilities through [template metaprogramming](http://en.wikipedia.org/wiki/Template_metaprogramming).

## Features

---
At this time (April 2014) the library is beging completely rewritten from scratch.  
The initial implementation of the library suffers for some scalability issues due to problems (Bad dessign decissions?) on the base dessign and modules of the library.

To solve that problems, the library was completly redesigned, focusing the implementation on high-order metaprogramming facilities to make developing new features easy and reduce (Avoid, if possible=) coupling on different features of the library.

The reimplementation is done on the [`reboot` branch](https://github.com/Manu343726/Turbo/tree/reboot). Its (currently) focused in a simple set of high-order features:

### Uniform expression evaluation facilities
The goal is to create a way to evaluate any kind of expression the library could work with.  
By convention, this library works with types only. There are no templates with value parameters, basic values are provided through boxing using wrappers like `std::integral_constant`. *Note that this is not required inside the implementation itself*.  
So an expression could be:
  1. A simple value (Like `int`).
  2. A *parametrized-expression*: A parametrized expression is just an expression composed from a set of components. Because this is a template metaprogramming library, the way to build expressions is through templates. So a parametrized expressions refers to any kind of template.
  3. A *functional expression*: This is a type of parametrized expression dessigned to return a value from a set of parameters. That is, a function.
  This library assumes that any expression with a `result` type member is a function. 

         //A simple expression
         using e1 = int; //e1 is a simple expression
     
         //A functional expression
         using e2 = tml::function<int>; //e2 is a functional expression
     
         //A more complex expression (A functional expression)
         using e3 = tml::transform<tml::list<int,float,double>,f<_1,_2>>;

To evaluate an expression, one should evaluate the entire set of parameters of a parametrized expression, and return the result if the expression is a functional expression. Thats what `tml::eval` is dessigned for:

    //Just a simple identity metafunction:
    template<typename T>
    struct identity
    {
      using result = T;
    }
    
    using expression = identity<int>;
    using result = tml::eval<identity>; //Compute the result of evaluating the expression.
    using result = tml::eval<identity<identity<int>>>; //result is int

Also, `tml::eval` could be used to take an expression and evaluate it with a new set of argumments. Following with the example above:

    using binded = tml::eval<expression,identity<float>>; //We evaluate expression with identity<float> instead of identity<int>

Or one could fill the expression with placeholders and evaluate the expression later when the argumments are aviable (Lazy evaluation):

    using expression = f<_1,_2,_3>; //_1,_2, and _3 are placeholders
    ...
    using result = tml::eval<expression,float,int,double>;

### Haskell-like let expressions

`tml::eval` allows you to evaluate an exsisting expression with other argumments, but it hasn't enought power to be usable in all situations.
For example, `tml::eval` only binds parameters of the main scope, so an expression with nested parametrized expressions can only be reevaluated specified the most enclosing parameter (See the tml::eval binding examples above).

Turbo probides `tml::let`, a high-order metafunction similar to haskell's `let`
Its purpose is to subsitute a value on an expression, given an specifiec variable to bind the value with:

    struct X{}; //A "variable"
    
    using expression = tml::let<X,int,tml::function<X>>; // expression is "tml::function<int>"

The power of let comes from its ability to parse the entire expression recursively, subsituting all ocurrences of the variable with the specified value:

    using expression = tml::let<X,float,f<X,int,tml::function<g<X,X>>>>; //Expression is f<float,int,tml::function<g<float,float>>>

Finally, Turbo extends that concept providing the `tml::multi_let` template,
a template dessigned as a let expression of multiple variables:

    using expression = tml::multi_let<X,Y,Z, //variables
                                      int,char,double, //values
                                      f<X,g<Y,Z>> //expression
                                     >;

`tml::multi_let` works currifying the multiple variable let into a chain of nested `tml::let` expressions. 

### Lambda expressions:

The ability of substituting a value in an expression provided by `tml::let` makes possible to create lambda expressions without any special effort. Turbo provides the `tml::lambda` template:

    //Gets a list with the sizes of the specified types
    using result = tml::transform<tml::lambda<_1,tml::size_of<_1>>,float,int,double>;

Turbo provides a set of features to simplify type manipulation and compile-time computations:

---

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


### Typelists

Turbo implements variadic-template based typelists through the `mpl::list` class:

    #include "list.hpp"
    #include "to_string.hpp"
    
    using list = mpl::list<bool,char,int,float,double>;
    
    int main()
    {
      std::cout << mpl::to_string<list>() << std::endl;
    }
> [bool,char,int,float,double]

#### List operations:
The library provides a set of list operations aviable for typelists: Splitting, concating, inserting, erasing, etc:

    #include "list.hpp"
    #include "to_string.hpp"
    
    using list = mpl::list<bool,char,int,float,double>;
    
    using index = mpl::index_of<list,float>; //index is mpl::size_t<3>
    using a = typename mpl::split<list,mpl::size_t<2>>::left;
    using b = typename mpl::split<list,mpl::size_t<2>>::right;
    using c = mpl::concat<b,a>;
    
    
    
    int main()
    {
      std::cout << mpl::to_string<list>() << std::endl;
      std::cout << mpl::to_string<a>() << std::endl;
      std::cout << mpl::to_string<b>() << std::endl;
      std::cout << mpl::to_string<c>() << std::endl;
      
    }
> [bool,char,int,float,double]
> [bool,char,int]
> [float,double]
> [float,double,bool,char,int]

#### Compile-time list sorting
Using the list operations showed above **Turbo implements a sorting metafunction<sup>1</sup> which can be used to sort types with the specified criteria**. For example:

    #include "list.hpp"
    #include "sort.hpp"
    
    using list = mpl::list<double,char,float,int,
    
    template<typename T , typename U>
    using comparer = mpl::boolean<(sizeof(T) >= sizeof<(U))>;
    
    using sorted_list = mpl::sort<list,comparer>;
    
    int main()
    {
      std::cout << mpl::to_string<sorted_list>() << std::endl;
    }
 >  [double,int,float,char]
 
 *[1] `mpl::sort` uses the quickosrt sorting algorithm by default, but this metafunction is parametrized to allow specifying other algorithms. The metafunction is defined as:*
 
     template<
              typename LIST , 
              template<typename,typename> class COMPARER = mpl::greather_than , 
              template<typename,template<typename,typename>class> class ALGORITHM = mpl::qsort
             >
     using mpl::sort = /* ... */
*so you can implement your own sorting algorithm and pass it to the metafunction. For example:*

     template<typename LIST , template<typename,typename> class COMPARER>
     struct my_stupid_sorting_algorithm
     {
            using result = LIST;
     };
    
     using list = mpl::list<int,char,bool>;
     using sorted_list = mpl::sort<list,mpl::less_than,my_stupid_sorting_algorithm>;


### Iterators and loops

Turbo implements the iterator dessign pattern to provide an easy way to trasverse ranges or intervals, such as numeric intervals or a part of a typelist. The library defines three types of iterators: 
 - **Forward iterators**: Are iterators dessigned to trasverse a range forwards (From begin to end). 
 - **Forward iterators**:  This type of iterators are dessigned to trasverse a range form its end to its begin (In the backwards direction).
 - **Bidirectional iterators [DEPRECATED]**: Are iterators which can advance in any direction.

The library provides a set of metafunctions to work with iterators: 
 - **Functions to get iterators**: Are functions to create iterators from things to trasverse it: `mpl::begin` , `mpl::end` , `mpl::rbegin` , `mpl::rend`.
 - **Functions to manipulate iterators**: `mpl::next` , `mpl::previous`[DEPRECATED].
All of the metafunctions above can be specialized to implement user-defined iterators. In fact the library provides specializations to work with typelists and integral numbers. For example:

      #include "iterators.hpp"
      #include "numeric_iterators.hpp"
      #include "list.hpp"
      #include "basic_types.hpp"
    
      using list  = mpl::list<bool,char,int,float>;
      using begin = mpl::begin<list>;
      using end   = mpl::end<list>;
    
      /* A metafunction to print values of a typelist: */
    
      teplate<typename BEGIN , typename END>
      struct print_list
      {
        static void execute()
        {
            std::cout << mpl::to_string<typename BEGIN::value>() << std::endl;
          
            print_list<mpl::next<BEGIN>,END>::execute();
        }
      };
    
      template<typename END>
      struct print_list<END,END>
      {
        static void execute() {}
      };
    
      /* Possible usage: */
    
      using printer = print_list<mpl::begin<list> , mpl::end<list>>;
      using partial_printer = print_list<decltype( mpl::begin<list>() + mpl::size_t<2>() )  , mpl::end<list>>;
    
    
      int main()
      {
        printer::execute();
        std::cout << std::endl;
        partial_printer::execute();
      }
> bool
> char
> int
> float
>
> int
> float

For integral types the library provides a set of utility functions to make iterators from integral values. The example above could be rewritten to work with unsigned ints:

    using printer = print_list<mpl::make_uinteger_backward_iterator<10> , mpl::make_uinteger_backward_iterator<0>>;
> 10  
9  
8  
7  
6  
5  
4  
3  
2  
1

#### Loops: for and for each

A common problem with template-meta-programming and variadic templates is to execute an operation over a set of values (types). That leads to witing recursive metafunctions everytime we need to do that kind of operations.   

**Turbo provides the metaloops `mpl::for_loop` and `mpl::for_each` to simplify the implementation of that kind of things**. 
That loops works through iterators: What the loops do is to execute the specified *kernel* (The operation to be performed) through the range represented by the iterators. Finally, the loop returns the result.  

#### `mpl::for_each`
This loop is dessigned to apply the specified kernel to every type from a set of types, and return a typelist filled with the set of applications.  Its equivalent "runtime" code is: 
 
  template<typename iterator_type , typename result_type>
  std::vector<result_type> for_each(iterator_type begin , iterator_type end , result_type(*)(typename iterator_type::value_type) kernel)
  {
    std::vector<result_type> output;
    
    for( auto& it = begin ; it != end ; ++it)
      output.push_back( kernel(*it) );
    
    return output;  
  }
 
A kernel is a metafunction of the form:
    
     template<typename CURRENT>
     struct kernel
     {
      using result = /* Operation involving CURRENT */
     };
In other words, a one parameter function.

What `mpl::for_each` returns is the list of applications, that is, the set of `kernel::result` from each type. 
For example:

    using list = mpl::list<bool,char,float,int,double>;
    using begin = mpl::begin<list>;
    using end = mpl::end<list>;
    
    //A kernel: It returns the size of the type specified
    template<typename T>
    struct compute_sizeof
    {
      using result = mpl::size_t<sizeof(T)>;
    };
    
    using result = mpl::for_each<begin,end,compute_sizeof>;
    
    int main()
    {
      std::cout << mpl::to_string<result>() << std::endl;
    }
 Output:
 > [1,1,4,4,8]
 
 Note that a kernel is a metafunction of one parameter which returns via a `result` alias. Thats exatly the signature of `mpl::function`, so you could use `mpl::function` to simplify the implementation of a kernel. The example above could be rewriten as:
 
     template<typename T>
     using compute_sizeof = mpl::function<mpl::size_t<sizeof(T)>>;
 
 In addition, you could specify a filter (A boolean predicate) to discard elements of the input which does not fullfill a requeriment.  Following with the example above:
 
     ...
     
     template<typename T>
     using filter = mpl::boolean<sizeof(T) % 2 == 0>;
     
     using result = mpl::for_each<begin,end,compute_sizeof,filter>;
     
     ...
Now the output is:
> [4,4,8]

#### `mpl::for_loop`

`mpl::for_loop` is dessigned to **execute iterative computations**, in other words, does a loop over a range, and the kernel does computations over that range, storing the result and using the previous value of the result. This could be viewed as a for loop with an aux varialbe which stores the result of the computation, and the body of that loop (The kernel acts as the body of the loop). For example:

  int result;
  
  for(auto& it = begin ; it != end ; ++it)
  {                  
    result = *it * result;
  }                  

So the kernel has two parameters: **The current value of the iterator and the previous value of the result**:

  template<typename CURRENT , typename PREVIOUS_RESULT>
  struct kernel
  {
    using result = /* ... */
  };
The loop passes the `result` of the current kernel application to the next iteration. So **the loop needs the initial value of the "aux variable"**. `mpl::for_loop` is defined as follows:

  template<typename BEGIN , typename END , typename INITIAL_VALUE , template<typename,typename> class KERNEL>
  using for_loop = /*...*/

In addition, a kernel of a for loop must define a public boolean constant that specifies if the loop should be aborted. In other words, **the user could specify a break condition for the loop through the kernel**:

  template<typename CURRENT , typename PREVIOUS_RESULT>
  struct kernel
  {
    using result = /* ... */
    static const bool abort = /* ... */
  };

An example of the use of `mpl::for_loop` could be the computation of the summation of a range of numbers:

  using begin = mpl::make_uinteger_forward_iterator<0>;
  using end   = mpl::make_uinteger_forward_iterator<10>;
  
  template<typename CURRENT_VALUE , typename PREVIOUS_RESULT>
  struct kernel : public mpl::no_abort_kernel //This defines the abort flag as false
  {
    using result = mpl::add<PREVIOUS_RESULT,CURRENT_VALUE>;
  };
  
  using result = mpl::for_loop<begin,end,mpl::uinteger<0>,kernel>;
  
  int main()
  {
    std::cout << mpl::to_string<result>() << std::cout;
  }
Output:
> 45

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

### Trigonometric functions
In addition to the arithmetic operations shared with the integral values, Turbo [implements compile-time trigonometric functions such as sine and cosine through Taylor-McLaurin series](https://github.com/Manu343726/Turbo/blob/dynamic_fixed_point/trigonometry.hpp):

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

#### Square-root
The library [implements a square root function](https://github.com/Manu343726/Turbo/blob/dynamic_fixed_point/sqrt.hpp), `math::sqrt`, computing the value through the Newton's method to aproximate function roots:

  using two = mpl::decimal<2>;
  using result = math::sqrt<two>;
  
  int main()
  {
    std::cout << mpl::to_string<result>() << std::endl;
  }
Output:
> 1,4142

### Compile-time matrix algebra
Turbo implements 3x3 and 4x4 matrices to provide **compile-time matrix algebra**.  It supports matrix addition, substraction, and multiplication. For example:

  #include "matrix3x3.hpp"
  
  using unity = math::unity3x3<mpl::decimal>
  using a = decltype( (unity() * unity()) * mpl::decimal<4>() );
  using b = decltype( a() + a() );
  
  int main()
  {
    std::cout << mpl::to_string<b>() << std::endl;
  }
> | 8 0 0 |  
| 0 8 0 |  
| 0 0 8 |

### Compile-time 2d/3d transformations
In adition to matrices, Turbo implements 2d/3d/4d vectors and provides **transformation matrices** such as rotations, scales, translations, etc. For example:

  #include "matrix4x4.hpp"
  #include "vector.hpp"
  
  using v1 = math::vec3<mpl::decimal<1> , mpl::decimal<1> , mpl::decimal<1>>;
  using translation = mpl::vector<mpl::decimal<1> , mpl::decimal<0> , mpl::decimal<0>>;
  using angle = decltype(math::pi() / mpl::decimal<2>());
  using transformation = decltype( math::translate<translation>() * mpl::rotate<angle,math::x_axis>() ); 
  using v2 = decltype( transformation() * v1() );
  
  int main()
  {
    std::cout << mpl::to_string<v1>() << std::endl;
    std::cout << mpl::to_string<v2>() << std::endl;
  }
> (1,1,1)  
(2,1,-1)
  

### Compile-time string manipulation
Turbo explodes C++11 generalized constant expressions to manipulate raw strings and create types (metavariables) which the library can work with using template metaprogramming. For example:

    #include "string.hpp"
    #include "to_string.hpp"
    
    //TURBO_STRING_VARIABLE() macro defines a new string metavariable which holds the specified string:
    
    TURBO_STRING_VARIABLE( hello , "hello " );
    TURBO_STRING_VARIABLE( world , "world!" );
    
    using hello_world = tml::concat<hello,world>;
    
    int main()
    {
        std::cout << tml::to_string<hello_world>() << std::endl;
    }

> hello world!

### Future features:
I'm currently working in a compile-time time manipulation library (`tml::chrono` namespace), based on the Standard `std::chrono` library.  
Also I'm working on compile-time random number generation facilities, using the features of `tml::chrono` as seed.
