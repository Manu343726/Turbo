Turbo [![Build Status](https://travis-ci.org/Manu343726/Turbo.svg?branch=reboot)](https://travis-ci.org/Manu343726/Turbo)
=====

*C++11 metaprogramming library*

## What is Turbo?

Turbo is a library dessigned to provide compile-time utilities through [template metaprogramming](http://en.wikipedia.org/wiki/Template_metaprogramming).

One of the key points of C++, and where its expresiveness power comes from, is its ability to define new language constructs and/or customize existing constructs, all leading in a simple and clear syntax (See for example DSELs).

But when dealing with the creation and/or manipulation of such constructs, nothing is simple nor clear. Template-meta-programming is powerfull, but suffers from a horrible syntax and the lack of high-level (meta)programming features.  
Some people claim that tmp seems like a compile-time pure functional language built inside C++ itself. But instead metaprogramming in C++ is more like a functional language with the worst syntax ever made (Despite LISP?).

This library aims to provide high-level constructs to aid with the **manipulation of the C++ type system**, and the creation and execution of **complex computations at compile-time**; all with a clear and uniform syntax.
Also the library takes advantage of such features and provides some usefull C++ 
(runtime) utilities, which are easy to implement thanks to Turbo. 

## What about Boost.MPL?

[Boost.MPL](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/index.html) is the de facto metaprogramming library for C++. But it suffers from two main problems which, in my oppinion, makes difficult to work with it:

 - **Is Boost**: The Boost libraries are one of the best C++ libraries in the world, but they base the implementation of their features on many many tricks they implemented (See Boost.Preprocessor, for example). The problem with Boost is that making a project depend on the Boost libraries could be a problem, its a huge library. My intention was to rely on standard features only, that is, something which works perfectly providing a C++11 compliant compiler only.

 - **Boost.MPL was designed on the C++98 era**: C++11 is like a reboot of the language, and like in other parts of the language, metaprogramming in C++11 is far easier than in C++98/03. The problem is that MPL was written for/using C++98/03 and its design is based in some cumbersome constructs which have been cleared and/or simplified a lot in C++11. Specifically, template aliases are a huge advantage when doing template-meta-programming, and the Turbo library relies on that in all of its features.

## Features

At this time (April 2014) the library is being completely rewritten from scratch.  
The initial implementation suffers from some scalability issues due to problems (Bad design decissions?) on the base design and modules of the library.

To solve that problems, the library was completly redesigned, focusing the implementation on high-order metaprogramming facilities to make developing new features easy, and reduce (Avoid, if possible=) coupling on different features of the library.

The reimplementation of the library is being developed at the [`reboot` branch](https://github.com/Manu343726/Turbo/tree/reboot). Its (currently) focused in a simple set of high-order features:

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
     using e3 = tml::transform<tml::list<int,float,double>,tml::size_of<_1>>;

To evaluate an expression, one should evaluate the entire set of parameters of a parametrized expression, and return the result if the expression is a functional expression. Thats what `tml::eval` is dessigned for:

    //Just a simple identity metafunction:
    template<typename T>
    struct identity
    {
      using result = T;
    };
    
    using expression = identity<int>;
    using result = tml::eval<expression>; //Compute the result of evaluating the expression.
    using result = tml::eval<identity<identity<int>>>; //result is int

Also, `tml::eval` could be used to take an expression and evaluate it with a new set of argumments. Following with the example above:

    using binded = tml::eval<identity<int>,float>; //We evaluate identity<> with float instead of int as parameter

Or one could fill the expression with placeholders and evaluate the expression later when the argumments are aviable (Lazy evaluation):

    using expression = f<_1,_2,_3>; //_1,_2, and _3 are placeholders
    ...
    using result = tml::eval<expression,float,int,double>;

Because the library wrks with types only, lazy evaluation of metafunctions should be done filling the metafunction with 
placeholders to properly instantiate it (See the example above).  
Turbo provides the `tml::lazy` template, dessigned to take a template parameter storing it, making possible to instantiate the
template later:

    using t = tml::lazy<tml::function>; //t stores a tml::function metafunction
    using instance = tml::lazy_instance<t,int>; //instance is tml::function<int>

Also `tml::lazy` could be used to do lazy evaluation of metafunctions. For example:

    using t = tml::lazy<tml::function>;
    using result = tml::eval<t,int>; //Result is int (The result of evaluating tml::function<int>)


For more information see the documentation inside `eval.hpp`.

### Haskell-like let expressions

`tml::eval` allows you to evaluate an exsisting expression with other argumments, but it hasn't enought power to be usable in all situations.
For example, `tml::eval` only binds parameters of the main scope, so an expression with nested parametrized expressions can only be reevaluated specifying the most enclosing parameter (See the tml::eval binding examples above).

Turbo probides `tml::let`, a high-order metafunction similar to haskell's `let`
Its purpose is to subsitute a value on an expression, given an specifiec variable to bind the value with:

    struct X{}; //A "variable"
    
    using expression = tml::let<X,int,tml::function<X>>; // expression is "tml::function<int>"

The power of let comes from its ability to parse the entire expression recursively, subsituting all ocurrences of the variable with the specified value:

    using expression = tml::let<X,float,f<X,int,tml::function<g<X,X>>>>; //Expression is f<float,int,tml::function<g<float,float>>>

Finally, Turbo extends that concept providing the `tml::multi_let` template,
a template dessigned as a let of multiple variables:

    using expression = tml::multi_let<X,Y,Z, //variables
                                      int,char,double, //values
                                      f<X,g<Y,Z>> //expression
                                     >;

`tml::multi_let` works currifying the multiple variable let into a chain of nested `tml::let` expressions. 

### Lambda expressions:

The ability of substituting a value in an expression provided by `tml::let` makes possible to create lambda expressions without any special effort. Turbo provides the `tml::lambda` template:

    //Gets a list with the sizes of the specified types
    using result = tml::transform<tml::lambda<_1,tml::size_of<_1>>,tml::list<float,int,double>>;

Multiple-variable lambda expressions are provided too:

    //Returns true if at least one element of the SEQUENCE evaluates the predicate P to true
    template<typename P , typename SEQUENCE>
    using any_of = tml::foldl<tml::multi_lambda<_1,_2 , tml::logical_or<_1,tml::eval<P,_2>>>,SEQUENCE>;
    
### High-order algorithms. Lists and iterators:

Turbo implements a set of high-order metafunctions as algorithms provided by the `algorithm.hpp` header.
Those algorithms mimic the most common functional high-order functions to provide building blocks for the rest
of the library:

    //Compute the adition of a set of numbers at compile-time
    template<typename... Ns>
    using sum = tml::foldl<tml::lambda<_1,_2 , tml::add<_1,_2>>,tml::zero,tml::list<Ns...>>;
    
Turbo is a C++ library, but since template-meta-programming seems like a functional language, Turbo takes inspiration from functional languages such as Haskell.  
For algorithms, C++ uses an iterator approach, when Haskell uses lists instead. Both approaches has advantages and cons,
so what approach we should use?

Turbo doesn't choose between, instead implements both using exactly the same interface! Turbo algorithms are designed to
work with sequences, and that sequences could be represented via typelists (`tml::list`) or iterators:

    using numbers = tml::integer_list<1,2,3,4,5>;
    
    using squared = tml::map<tml::lambda<_1 , tml::mul<_1,_1>>,numbers>;
    using squared = tml::map<tml::lambda<_1 , tml::mul<_1,_1>>,tml::begin<numbers>,tml::end<numbers>>;

The library provides the header `iterator.hpp`, which implements metafunctions for iterators manipulation, such as `tml::begin`, `tml::end` , `tml::rbegin`, `tml::next` , `tml::deref` , etc. The header provides the declaration of
the metafunctions, aliases, and the implementation metafunctions, all ready to be highly customizable by the user. This allows to write iterator-ready custom types specializing a small set of metafunctions only. When its done, everything automagically works!

By default Turbo provides an iterator implementation for typelists (See the example above) and integral values (See the `numeric_iterators.hpp` header):

    //Returns a list filled with the numbers on the interval [begin,end)
    template<int begin , int end>
    using integer_range = tml::map<tml::function<_>,
                                   tml::forward_iterators::make_int<begin>,
                                   tml::forward_iterators::make_int<end>
                                  >;
                                   

### TMP-aware static asserting:

The standard `static_assert()` expects a boolean value as asserting condition. In mostly situations (Even using the Standard Library metaprogramming facilities only) that condition comes in the form of a boolean type (Like `std::integral_constant<bool,true>`).   
Turbo implements the macro `TURBO_ASSERT()` which is dessigned to work with such types without needing to extract the value via the `::value` member:

    TURBO_ASSERT( (std::is_integral<int>) , "What happened????" );

### Compilation-time static warning:

The only standard way to throw warnings during compilation is the `#warning` macro. But this warnings are checked during preprocessing time, and what a C++ (meta)programmer needs is a way to generate warnings depending on compile-time values
and/or templates.

Turbo implements a `STATIC_WARNING()` macro, which generates a warning at template instantation phase. This feature is based 
in a `deprecated` attribute trick, which will be standard in C++14. At this time, Turbo uses compiler-specific attributes.

### Uniform multiple-SFINAE facilities:

The Standard template to do SFINAE, `std::enable_if`, disables the instantation of a template if a certain boolean condition is
not guaranteed. As in the `static_assert()` case explained above, `std::enable_if` expects a boolean value as condition. Also,
the member type `::type` of `std::enable_if` should be explicitly referenced via the common and cumbersome `typename ::type` construction.

Turbo provides the macros `TURBO_ENABLE_IF()` and `TURBO_DISABLE_IF()`, which makes SFINAE clean and easy. For example:

    template<typename T , typename SFINAE_FLAG = tml::sfinae_result>
    struct f;

    template<typename T>
    struct f<T , TURBO_ENABLE_IF( std::is_floating_point<T> )>
    {};

There are cases where a template should be enabled/disabled depending in many conditions. This could be achieved passing 
a complex boolean expression to `std::enable_if` (Or `TURBO_ENABLE/DISABLE_IF()`).   
Instead Turbo implements a so-called *sfinae container*, that is, a template dessigned to store multiple SFINAE entities like `std::enable_if` and behave itself as a big `enable_if`. That template is enabled (ie declares a `::type` member) if and only
if all the SFINAE entities passed are enabled.  

Also Turbo provies some macros, `TURBO_SFINAE_ALL()` , `TURBO_SFINAE_ANY()`, `ENABLE_IF()`, and `DISABLE_IF()` to build such sfinae containers easily. The combination of these features makes possible to write multiple-condition SFINAE expressions in a clear and concise syntax:

    //Following with the f example above, now we define another f specialization:

    template<typename T>
    struct f<T , TURBO_SFINAE_ALL( DISABLE_IF( std::is_floating_point<T> ),
                                   ENABLE_IF( std::is_default_constructible<T>)
                                 )>
    {
    
    };


### Runtime access to compile-time computations via the `tml::to_runtime<T>()` function:

The function `tml::to_runtime<T>()` returns a runtime constant equivalent to the spefied compile-time value `T`. Its dessigned to provide a clear interface between the compile-time and runtime sides of a program. For example:

    //Compute the range of ints [10,20) at compile-time
    using numbers = tml::integer_range<10,20>;
    
    //Print that numbers at runtime:
    for( int i : tml::to_runtime<numbers>() )
        std::cout << i << " ";

> 10 11 12 13 14 15 16 17 18 19

## Known issues:

The features explained above have some implementation issues (Working on...):

### Template specialization  priority issues. A ISO C++ Standard bug?

The initial implementation of `tml::eval` consisted on three different cases (Partial specializations), one for each kind of expression the library is cappable of evaluate:

 1. **Simple values**: The result of evaluating a value is the value itself
     
         using result = tml::eval<tml::Int<0>>; //result is Int<0>
         using result = tml::eval<int>; //result is int
 
 2. **Parametrized expressions**: Parametrized expressions are not functions, but their parameters could be anything, so they must be evaluated

         using vector = tml::eval<std::vector<tml::function<int>>>; // vector is std::vector<int>

 3. **Functional expressions**: Same as parametrized expressions, but they have a result which should be computed (Extracted)

         using myint = tml::eval<tml::function<int>>; //myint is int

 4. **Functional expressions with binded argumments**: `tml::eval` could be used to reevaluate an existing (Instanced) functional expression
    with a new set of parameters

         using result = tml::eval<tml::function<int>,double>; //result is double

In addition to this generic cases, the user could explicitly specialize the implementation of `tml::eval` (The internal template `tml::impl::eval`)
to make `tml::eval` work in a custom and specific way. For example:

     struct foo {};
     
     //We customize tml::eval saying the result of evaluating 'foo' is 'int'
     template<>
     struct eval<foo> : public tml::function<int>
     {};


When the specialized expression is complex (Like a template `template<typename T> struct bar{};`) that specialization has conflicts with the default generic specializations.
The *common sense* says that our custom specialization should be instanced, because `bar<T>` is more specialized than `F<T>` (The generic functional case).  
Instead, the generic specialization is instanced or the compilation fails due to ambiguous template specializations (Depending on the compiler).

This situation [was discussed](http://stackoverflow.com/questions/23393962/partial-template-template-based-specialization-vs-explicit-partial-template-spec) for two weeks, and after some effort we conclused this is an issue on the wording of the ISO Standard, specifically:

> If for each type being considered a given template is at least as specialized for all types, and  
> more specialized for some set of types and the other template is not more specialized for any types, or  
> {the other template} is not at least as specialized for any types,  
> then the given template is more specialized than the other template.  

An ambiguity problem  very similar to [an official ISO C++ issue](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#1705) which has no official resolution (At June 2014).

*The situation was solved using SFINAE and a custom registry metafunction which specifies if a certain kind of       expression overrides the default behaviour of `tml::eval`. Note that its only a workaround, the bug on the template specialization rules is still there.*



### `tml::impl::multi_lambda` evaluation doesn't work on GCC
The multiple-variable lambda template is defined as follows:
     
      template<typename... VARIABLES , typename BODY>
      struct multi_lambda
      {
          template<typename... ARGS>
          using result = tml::eval<tml::multi_let<VARIABLES...,
                                                  ARGS...,
                                                  BODY
                                                 >
                                  >;
      };

Later `tml::impl::multi_lambda` overrides `tml::eval` to call the `::result` template alias properly:

      template<typename... VARIABLES , typename BODY>
      struct overrides_eval<tml::impl::multi_lambda<VARIABLES...,BODY>> : public tml::true_type
      {};
 
      template<typename... VARIABLES , typename BODY , typename... ARGS>
      struct eval<tml::impl::multi_lambda<VARIABLES...,BODY>,tml::list<ARGS...>> :
         public tml::function<typename tml::impl::multi_lambda<VARIABLES...,BODY>::template result<ARGS...>>
      {};


*The evaluation of that lambda does not work on GCC. After some discussions and tests I'm sure this code is valid, seems like a GCC bug.
This project was reconfigured to use the LLVM/CLang toolchain, where this code works perfectly.*



### The `TURBO_ASSERT()` macro overloading doesn't work on LLVM/CLang
`TURBO_ASSERT()` is a macro "overloaded", making possible to pass one or two parameters depending on the use case.  
The macro overloading is an old trick to define macros with different number of parameters, but with the same name.
`TURBO_ASSERT()` was implemented and tested using GCC 4.8.2, but the overloading doesn't work properly on LLVM/CLang toolchain for some 
reason.

*Currently fixed, was a bug on the `SELECT_ARG_3(...)` macro.*


### Lambda body and placeholders evaluation
As explained above in the `Features` entry, Turbo implements lambda expressions as Haskell-like let expressions where the lambda variables are substituted with the value of the parameters at the point of lambda evaluation:

     using l = tml::lambda<_1 , tml::function<_1>>;
     using result = tml::eval<l,int>; //result is int

Even if `tml::eval` is specialized to take care of placeholders, there are cases when expressions depending on `tml::eval` evaluation are not correctly evaluated because 
they have placeholders. Consider this example:

     template<typename F , typename SEQ>
     using any_of = tml::foldl<tml::lambda<_1,_2 , logical_or<_1,tml::eval<F,_2>>>,tml::false_type,SEQ>;

This sentence defines a metafunction `any_of`, which returns true if almost one element of a sequence evaluates to true certain predicate.
Its implemented using a metafunction provided by the Turbo "algorithm.hpp" header, a Haskell-like `foldl` metafunction (Similar to `std::accumulate()`).
The combination metafunction passed to `tml::foldl` is written in the form of a binary lambda expression, which computes the logical or between the current state of the 
computation and the current element of the sequence. "Readable" functional programming at compile-time in C++. Cool, isn't?

But that doesn't work. Note that the `tml::eval` written inside the lambda body is instanced (Executed) before the substitution of the lambda variables (The placeholders).
To deal with that situations, a template `tml::delayed_eval` (`tml::deval`) was designed to hold a `tml::eval`-like expression inside let expressions. During the let execution, `tml::delayed_eval` 
is substituted by `tml::eval` **after variable substitution**. The correct `tml::any` implementation should be:

      template<typename P , typename SEQUENCE>
      using any = tml::foldl<P,tml::lambda<_1,_2 , tml::logical_or<_1,tml::deval<P,_2>>>>,tml::false_type,SEQUENCE>;

This solution successfully solved the problem on unary lambdas, but it doesn't work on multiple-variable lambda expressions, probably because of the curryfication process.

*Fixed, it was a bug on the currifier*
   


### Design flaws on the expression evaluation system

The Turbo expression evaluation mechanism is based on the processing of
any valid expression (Simple, parametrized, or functional) transforming the input expression to compute its value.
Thus the evaluation should be completely separated from the instantation of the original expression. This is why
some workarounds (Like chameleons) are needed: A non-evaluated part of an expression should be a valid parameter for
the enclosing expression, until the expression evaluation (After the parameter is evaluated and will be valid).
This scheme works well with total metafunctions (Metafunctions with a defined result for any input), but not for partial
metafunctions, like the algebraic operations. Consider this:

    using result = tml::eval<tml::add<tml::div<tml::Int<10>,
                                               tml::Int<2>
                                              >,
                                      tml::Int<1>
                                     >
                            >;

The algebraic metafunctions tml::add and tml::div are defined (Specialized) for numeric values only, so this expression doesn't even compile (There is no tml::add specialization to add a tml::div instance and a tml::Int). Even if the expression seems to be well-formed (Is (10/2) + 1), is not because the semantic of the expression is not (10/2)+1 until the evaluation. Before is just a template which should be instanced. And there is no specialization.

The good solution was to decouple completely the correctness (Is well-formed or not) of an expression from its instantation. Note that Turbo expressions are processed in two phases (First the expression is instantiated, and that instantation should be correct. Later the instanced
expression is evaluated).
To solve the problem all Turbo metafunctions are declared as total functions (Even if they are not). The default template of a
partial metafunction could inherit from the tml::non_evaluated_function_chameleon type to behave like a total function and make
the instantation (1st phase of expression processing) possible.



## README content of the original library implementation:

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
