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

#include "basic_types.hpp"
#include "for_loops.hpp"

#ifndef PRIME_FILTER_HPP
#define	PRIME_FILTER_HPP

namespace math
{
    template<typename A , typename B>
    using is_divisible = tb::boolean<A::value % B::value == 0>;
    
    namespace 
    {
        template<typename T>
        struct _is_prime;

        template<typename T>
        struct _is_prime
        {
            using numbers = tb::for_each<tb::make_uinteger_forward_iterator<2> , tb::make_uinteger_forward_iterator<T::value> , tb::function>;

            template<typename NUMBERS , typename = tb::no_type>
            struct __is_prime;

            template<typename HEAD , typename... TAIL>
            struct __is_prime<tb::list<HEAD,TAIL...> , tb::no_type> : public tb::boolean<!is_divisible<T,HEAD>::value && __is_prime<tb::list<TAIL...>>::value> {}; 

            template<typename foo>
            struct __is_prime<tb::empty_list,foo> : public tb::true_type {};

            using result = tb::boolean<__is_prime<numbers>::value>;
        };

        template<>
        struct _is_prime<tb::uinteger<0>> : public tb::function<tb::false_type> {};

        template<>
        struct _is_prime<tb::uinteger<1>> : public tb::function<tb::false_type> {};

        template<>
        struct _is_prime<tb::uinteger<2>> : public tb::function<tb::true_type> {};
        
        
        /* Specialization for integers */
        
        template<int VALUE>
        struct _is_prime<tb::integer<VALUE>> : public _is_prime<tb::uinteger<(unsigned int)VALUE>> {};
    }
    
    template<typename T>
    using is_prime = typename _is_prime<T>::result;
}

#endif	/* PRIME_FILTER_HPP */

