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

#ifndef VALUE_T_HPP
#define	VALUE_T_HPP

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

namespace tb
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
    using is_value = tb::value_t<bool,implementation__is_value::_is_value<T>::value>;
}

#endif	/* VALUE_T_HPP */

