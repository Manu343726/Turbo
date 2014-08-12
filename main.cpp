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


using a = tml::floating::integer<0>;
using b = tml::floating::decimal<0,5>; 

using sqrt_ = tml::eval<tml::sqrt<tml::floating::integer<2>>>;
using number = tml::eval<tml::mul<sqrt_,sqrt_>>;

int main()
{
    std::cout << tml::to_string<sqrt_>() << std::endl;
    std::cout << tml::to_runtime<sqrt_>() << std::endl;
    std::cout << tml::to_string<number>() << std::endl;
    std::cout << tml::to_runtime<number>() << std::endl;
}
