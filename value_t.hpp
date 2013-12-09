/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013, Manuel Sánchez Pérez                                    *
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

#ifndef VALUE_T_HPP
#define	VALUE_T_HPP

#include "operators.hpp"

#include <type_traits> //std::common_type

namespace implementation__is_value
{
    template<typename T>
    struct _is_value
    {       
    private:
        typedef struct { char c[1]; } yes;
        typedef struct { char c[2]; } no;
        
        template<typename C> static yes test(typename C::is_value_flag*);
        template<typename C> static no  test(...);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof( yes );
    };
}

namespace mpl
{
    template<typename T , T val>
    struct value_t
    {
    private:
        struct _is_a_value {};
    public:
        static const T value = val;
        using value_type = T;
        
        using is_value_flag = _is_a_value;
        
        const constexpr T operator()() { return val; }
    }; 
    
    template<typename T>
    using is_value = mpl::value_t<bool,implementation__is_value::_is_value<T>::value>;
    
    
    /* identities */
    
    template<typename T , T a>
    struct one_t<mpl::value_t<T,a>> : public mpl::function<mpl::value_t<T,1>> {};
    
    template<typename T , T a>
    struct zero_t<mpl::value_t<T,a>> : public mpl::function<mpl::value_t<T,0>> {};
    
    /* Arithmetic operators */
    
    template<typename T , typename U , T a , U b>
    struct add_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , a + b>> {};
    
    template<typename T , typename U , T a , U b>
    struct sub_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , a - b>> {};
    
    template<typename T , typename U , T a , U b>
    struct mul_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , a * b>> {};
    
    template<typename T , typename U , T a , U b>
    struct div_t<mpl::value_t<T,a>,mpl::value_t<U,b>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , a / b>> {};
    
    
    /* logical operators */
    
    template<bool b>
    struct logical_not_t<mpl::value_t<bool,b>> : public mpl::function<mpl::value_t<bool,!b>> {};
    
    template<bool lhs , bool rhs>
    struct logical_and_t<mpl::value_t<bool,lhs>,mpl::value_t<bool,rhs>> : public mpl::function<mpl::value_t<bool,lhs && rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_or_t<mpl::value_t<bool,lhs>,mpl::value_t<bool,rhs>> : public mpl::function<mpl::value_t<bool,lhs || rhs>> {};
    
    template<bool lhs , bool rhs>
    struct logical_xor_t<mpl::value_t<bool,lhs>,mpl::value_t<bool,rhs>> : public mpl::function<mpl::value_t<bool,lhs ^ rhs>> {};
    
    /* comparison operators */
    
    template<typename T , typename U , T lhs , U rhs>
    struct equal_t<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> : public mpl::function<mpl::value_t<bool,lhs == rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct less_than_t<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> : public mpl::function<mpl::value_t<bool,(lhs < rhs)>> {};
    
    /* bitwise operators */
    
    template<typename T , T a>
    struct bitwise_not_t<mpl::value_t<T,a>> : public mpl::function<mpl::value_t<T,~a>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_and_t<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , lhs & rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_or_t<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , lhs | rhs>> {};
    
    template<typename T , typename U , T lhs , U rhs>
    struct bitwise_xor_t<mpl::value_t<T,lhs>,mpl::value_t<U,rhs>> : public mpl::function<mpl::value_t<typename std::common_type<T,U>::type , lhs ^ rhs>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_leftshift_t<mpl::value_t<T,value>,mpl::value_t<U,shift>> : public mpl::function<mpl::value_t<T,( value << shift )>> {};
    
    template<typename T , T value , typename U , U shift>
    struct bitwise_rightshift_t<mpl::value_t<T,value>,mpl::value_t<U,shift>> : public mpl::function<mpl::value_t<T,( value >> shift )>> {};
    
    
}

#endif	/* VALUE_T_HPP */

