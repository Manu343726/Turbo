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

#include "eval.hpp"
#include "placeholders.hpp"
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
#include "stl_adapters.hpp"
#include "bind.hpp"
#include "to_runtime.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>

using namespace tml::placeholders;

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
using t6 = tml::eval<tml::multi_lambda<_1,_2,_3 , f<_1,_2,_3>>,Int<1>,Int<2>,Int<3>>;

constexpr bool a = tml::is_function<Int<0>>::value;


TURBO_ASSERT((std::is_same<TURBO_SFINAE(
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

template<typename T>
using this_is_not_java_vector = std::vector<tml::eval<tml::stl::function<std::remove_pointer<T>>>>;

template<typename T>
using this_is_how_you_should_do_polymorphism = std::vector<std::unique_ptr<tml::stl::eval<std::remove_pointer<T>>>>;

TURBO_ASSERT(( std::is_same<std::vector<int>,this_is_not_java_vector<int*>> ));



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
}
