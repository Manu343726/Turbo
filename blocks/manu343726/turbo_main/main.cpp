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

#include "manu343726/turbo_core/turbo_core.hpp"
#include "manu343726/turbo_computing/float.hpp"

#include <type_traits>
#include <iostream>
#include <iomanip>

using namespace tml::placeholders;
using namespace tml::runtime::placeholders;


using a = FLOAT(0.5);

using l = tml::lambda<_1 , tml::add<_1,_1> >;
using b = tml::bind<tml::function,int>;
using r = tml::eval<l(tml::Int<1>)>;

int main()
{
    std::cout << std::setprecision(10) << std::defaultfloat
              << tml::to_string<r>()  << std::endl
              << tml::to_runtime<a>() << std::endl;
            
}
