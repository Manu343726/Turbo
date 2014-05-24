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

#include <iostream>

using namespace tml::placeholders;

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
using t6 = tml::eval<tml::multi_lambda<f<_1,_2,_3>,_1,_2,_3>,Int<1>,Int<2>,Int<3>>;

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

using res2 = tml::eval<tml::multi_lambda<_1 , 
                                         tml::delayed_eval<tml::lazy<tml::function>,_1>
                                        > , 
                       Int<0>
                      >;

using res3 = tml::eval<tml::multi_let<_1,tml::Int<0> , tml::delayed_eval<tml::lazy<tml::function>,_1>>>;


template<typename LHS , typename RHS>
struct logical_or : public tml::function<tml::boolean<LHS::value || RHS::value>>
{};

template<typename F , typename SEQ>
using any_of = tml::foldl<tml::multi_lambda<_1,_2 , logical_or<_1,tml::delayed_eval<F,_2>>>,tml::false_type,SEQ>;

template<typename N>
struct odd : public tml::function<tml::boolean<(N::value % 2) == 0>>
{};

using map_test = tml::transform<tml::lazy<odd>,tml::list<Int<0>,Int<2>,Int<4>>>;
using any_of_test = any_of<tml::lazy<odd>,tml::list<Int<0>,Int<2>,Int<4>>>;

int main()
{
    std::cout << tml::impl::demangle( typeid( res2 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( res3 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( map_test ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( any_of_test ).name() ) << std::endl;
}
