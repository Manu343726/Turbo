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

#include "integral.hpp"
#include "utils/assert.hpp"
#include "warning.hpp"
#include "impl/demangle.hpp"
#include "integral_iterators.hpp"
#include "fixed_point.hpp"
#include "float.hpp"
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


using one = tml::one<tml::floating::number<__,__,__>>;
using one_denorm = tml::floating::shift<one,tml::Int<-31>>;
using two   = tml::eval<tml::add<one,one>>;
using three = tml::eval<tml::add<two,one>>;
using four  = tml::eval<tml::mul<two,two>>;

//Generate N powers of two using floating-point at compile-time:
template<std::size_t N>
using generate_numbers = tml::foldl<tml::lambda<_1,_2 , tml::lists::func::push_front<_1,tml::add<tml::lists::func::head<_1>,one>>> , tml::list<one> , tml::integral_forward_iterators::make_size_t<0> , tml::integral_forward_iterators::make_size_t<N>>;


int main()
{
    using numbers = generate_numbers<400>;
    
    for( double n : tml::to_runtime<numbers>() )
        std::cout << n << std::endl;

}
