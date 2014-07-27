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

#define TURBO_DEBUG_FP_DECIMAL_PART 3


#include "eval.hpp"
#include "placeholders.hpp"
#include "runtime_placeholders.hpp"
#include "let_expressions.hpp"

#include "lazy.hpp"
#include "lambda.hpp"
#include "algorithm.hpp"

#include "integral_lists.hpp"

#include "basic_types.hpp"
#include "utils/assert.hpp"
#include "warning.hpp"
#include "impl/demangle.hpp"
#include "integral_iterators.hpp"
#include "fixed_point.hpp"
#include "bind.hpp"
#include "to_runtime.hpp"
#include "curry.hpp"
#include "type_traits.hpp"
#include "overloaded_function.hpp"
#include "static_if.hpp"
#include "decimal.hpp"

#include "polymorphic_container.hpp"
#include "pipelined_function.hpp"

#include <type_traits>
#include <iostream>

using namespace tml::placeholders;
using namespace tml::runtime::placeholders;


#define RUN_UNIT_TESTS

#ifdef RUN_UNIT_TESTS

template<typename... ARGS>
struct f
{
    using result = tml::list<ARGS...>;
};

using t1 = tml::eval<Int<0>>;
using t2 = tml::eval<f<_1,_2,_3>,Int<1>,Int<2>,Int<3>>;
using t3 = tml::eval<tml::lazy<f>,Int<1>,Int<2>,Int<3>>; 
using t4 = tml::eval<tml::lambda<_1,f<_1,Int<2>,Int<3>>>,Int<1>>;
using t5 = tml::eval<tml::multi_let<_1,_2,_3,Int<1>,Int<2>,Int<3>,f<_1,_2,_3>>>;
using t6 = tml::eval<tml::lambda<_1,_2,_3 , f<_1,_2,_3>>,Int<1>,Int<2>,Int<3>>;

constexpr bool a = tml::is_function<Int<0>>::value;


TURBO_ASSERT((std::is_same<TURBO_SFINAE_ALL(
                                            DISABLE_IF(tml::is_function<Int<0>>) ,
                                            DISABLE_IF(tml::overrides_eval<Int<0>>)
                                           ),tml::sfinae_return>));

TURBO_ASSERT((std::is_same<t1,tml::integer<0>>));
TURBO_ASSERT((std::is_same<t2,tml::integer_list<1,2,3>>));
TURBO_ASSERT((std::is_same<t3,tml::integer_list<1,2,3>>));
TURBO_ASSERT((std::is_same<t4,tml::integer_list<1,2,3>>));
TURBO_ASSERT((std::is_same<t5,tml::integer_list<1,2,3>>));
TURBO_ASSERT((std::is_same<t6,tml::integer_list<1,2,3>>));

#endif /* RUN_UNIT_TESTS */

template<typename N>
struct odd : public tml::function<tml::boolean<(N::value % 2) == 0>>
{};

#ifndef RANGE_END
#define RANGE_END 20
#endif

using numbers = tml::integer_list<0,1,2,3,4,5>;
using numbers2 = tml::integer_range<0,RANGE_END>;


using map_test    = tml::map<tml::lazy<odd>,numbers2>;
using any_of_test = tml::any<tml::lazy<odd>,tml::iterator::begin<numbers> , tml::iterator::end<numbers>>;
using all_of_test = tml::all<tml::lazy<odd>,tml::integer_list<0,1,2,3,4,5>>;


using x = tml::fsingle<(1 << 7)>; //0.5
using y = tml::fsingle<(1 << 9)>; //2.0
using z = tml::eval<tml::div<x,y>>; //0.25?
using w = tml::eval<tml::mul<z,y>>; //0.5?
using q = tml::eval<tml::div<y,tml::fsone>>;

TURBO_ASSERT((tml::is_stl_function<std::remove_pointer<int*>>));

using tt = tml::eval<tml::lazy<std::remove_pointer,int*>>;



template<typename T>
struct quux;

template<>
struct quux<char>
{};


//Conditional selection of potentially ill-formed types! Thanks to Turbo tml::lazy its easy:
using ok = tml::lazy_instance<tml::conditional<tml::false_type,
                                               tml::lazy<quux,char>,
                                               tml::lazy<quux,bool>
                                              >
                             >;

TURBO_ASSERT(( std::is_same<ok,quux<bool>> ));


template<typename A , typename B , typename C>
struct third_arg : public tml::function<C>
{};


using b = tml::bind<third_arg , _3,_2,_1>;
using bcall = tml::eval<b,char,char,int>; //Equivalent to tml::eval<third_arg<int,bool,char>>

TURBO_ASSERT(( std::is_same<bcall,char> ));

template<typename... ARGS>
struct fooquux : public tml::function<tml::util::pack_length<ARGS...>>
{};

using u = tml::uncurry<fooquux>;
using resu = tml::eval<u,tml::list<int,float,char>>;

IF( (tml::true_type) THEN
{
    
}
ELSE
{

}
END_IF)


using ten = tml::Int<10>;
using two = tml::Int<2>;
using hundred = tml::eval<tml::pow<ten,two>>;
using v1_100  = tml::eval<tml::div<tml::one<tml::fdouble<__>>,tml::to_fdouble<hundred>>>;

//TURBO_ASSERT((tml::equal<tml::Int<100>,hundred>));


using r = tml::eval<L(tml::div)(tml::Int<4>,tml::Int<2>)>;

struct X {};

using let_result_bind = tml::multi_let<X,tml::Int<0>,tml::bind<fooquux,X>>;

int fibo( int i )
{
    tml::runtime::pipelined_function<int(int)> f;
    
    return f.begin([](int ii)
     {
         std::cout << "Starting... ( i = " << ii << " )\n";
     })
     .body([&](int ii)
     {
         if( ii > 1 )
             return f(ii - 1) + f(ii - 2);
         else
             return 0;
     })
     .end([](int ii)
     {
         std::cout << "End ( i = " << ii << " )\n";
     })( i );
}




int main()
{
    std::cout << tml::to_string<numbers>() << std::endl;
    std::cout << tml::to_string<map_test>() << std::endl;
    std::cout << tml::to_string<any_of_test>() << std::endl;
    std::cout << tml::to_string<all_of_test>() << std::endl;
    std::cout << tml::to_string<numbers2>() << std::endl;
    
    std::cout << tml::to_string<x>() << std::endl;
    std::cout << tml::to_string<y>() << std::endl;
    std::cout << tml::to_string<z>() << std::endl;
    std::cout << tml::to_string<w>() << std::endl;
    
    std::cout << tml::to_string<bcall>() << std::endl;
    
    for( auto v : tml::to_runtime<numbers>() )
    {
        std::cout << v << std::endl;
    }
    
    std::cout << "(" << std::boolalpha
              << std::get<0>( tml::to_runtime<tml::list<tml::Char<'a'>,tml::Bool<true>,tml::Int<0>>>() ) << "," 
              << std::get<1>( tml::to_runtime<tml::list<tml::Char<'a'>,tml::Bool<true>,tml::Int<0>>>() ) << "," 
              << std::get<2>( tml::to_runtime<tml::list<tml::Char<'a'>,tml::Bool<true>,tml::Int<0>>>() ) << ")" << std::endl;
    
    std::cout << tml::to_string<resu>() << std::endl;
    
    auto f = [](int , int){};
    auto g = tml::runtime::make_overloaded_function( [](int){} , [](std::string){} , [](std::ostream&){} );
    
    std::cout << tml::to_runtime<tml::is_valid_call<decltype(f),char>>() << std::endl;
    std::cout << tml::to_runtime<tml::is_valid_call<decltype(f),int,int>>() << std::endl;
    std::cout << tml::to_runtime<tml::is_valid_call<decltype(g),int>>() << std::endl;
    std::cout << tml::to_runtime<tml::is_valid_call<decltype(g),std::vector<int>>>() << std::endl;
    
    std::cout << tml::to_string<tml::one<tml::Int<__>>>() << std::endl;
    
    std::cout << tml::to_runtime<tml::decimal_fdouble<126,2>>() << std::endl;
    std::cout << tml::to_runtime<v1_100>() << std::endl;
    
    
    struct base 
    { 
      virtual int f(int x = 0)const=0; 
    }; 

    struct derived1:base 
    { 
      derived1(int n = 0):n(n){} 
      virtual int f(int x)const{return 126*x*n;} 

      int n; 
    }; 

    struct derived2:base 
    { 
      derived2(int n = 0):n(n){} 
      virtual int f(int x)const{return x+n;} 

      int unused,n; 
    }; 
    
    for( std::size_t ii = 0 ; ii < 1 ; ++ii )
    {
        tml::runtime::poly_container<base> poly;
        constexpr const std::size_t test_size = 10000000;

        for( std::size_t i = 0 ; i < test_size ; ++i )
            poly.insert( derived1{12} );

        for( std::size_t i = 0 ; i < test_size ; ++i )
            poly.insert( derived2{22} );

        auto begin = std::chrono::high_resolution_clock::now();

        for( auto& e : poly )
            e.f(55);

        auto poly_elapsed = std::chrono::high_resolution_clock::now() - begin;

        std::vector<std::unique_ptr<base>> v;

        for( std::size_t i = 0 ; i < test_size ; ++i )
            v.emplace_back( new derived1{11} );

        for( std::size_t i = 0 ; i < test_size ; ++i )
            v.emplace_back( new derived2{22} );

        std::random_shuffle( std::begin( v ) , std::end( v ) );

        begin = std::chrono::high_resolution_clock::now();

        for( auto& e : v )
            e->f(55);

        auto OO_elapsed = std::chrono::high_resolution_clock::now() - begin;

        std::cout << "Classic std::vector: "          << std::chrono::duration_cast<std::chrono::milliseconds>( OO_elapsed ).count()   << " ms" << std::endl;
        std::cout << "tml::runtime::poly_container: " << std::chrono::duration_cast<std::chrono::milliseconds>( poly_elapsed ).count() << " ms" << std::endl;
        
        
    }
    
    fibo( 10 );
    
    
    
    using filter_lazy = tml::lazy<tml::filter>;
    using filer_eval = tml::eval<filter_lazy,tml::lambda<_1 , tml::true_type> , numbers>;
    
    std::cout << tml::to_string<let_result_bind>() << std::endl;
}
