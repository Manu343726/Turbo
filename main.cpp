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

template<typename N>
struct odd : public tml::function<tml::boolean<(N::value % 2) == 0>>
{};

using numbers = tml::integer_list<0,1,2,3,4,5>;

using begin = tml::integral_forward_iterators::make_char<'a'>;
using end   = tml::integral_forward_iterators::make_char<'z'+1>;
using numbers2 = tml::transform<tml::lazy<tml::function>,begin,end>;

using map_test    = tml::map<tml::lazy<odd>,numbers2>;
using any_of_test = tml::any<tml::lazy<odd>,tml::iterator::begin<numbers2> , tml::iterator::end<numbers2>>;
using all_of_test = tml::all<tml::lazy<odd>,tml::integer_list<0,1,2,3,4,5>>;

int main()
{
    std::cout << tml::to_string<numbers>() << std::endl;
    std::cout << tml::to_string<map_test>() << std::endl;
    std::cout << tml::to_string<any_of_test>() << std::endl;
    std::cout << tml::to_string<all_of_test>() << std::endl;
    std::cout << tml::to_string<numbers2>() << std::endl;
}
