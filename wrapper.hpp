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

#ifndef WRAPPER_HPP
#define	WRAPPER_HPP

#include "operators.hpp"
#include "to_string.hpp"


namespace tb
{
    template<typename T , typename HASH = tb::no_type>
    struct wrapper_t : public tb::function<T>
    {
        using type = T;
        using hash = HASH;
    };
    
    template<typename T , typename HASH = tb::no_type>
    using wrapper = wrapper_t<T,HASH>;
    
    template<typename T , typename HASH>
    struct to_string_t<tb::wrapper<T,HASH>> : public to_string_t<T> {};
}

#endif	/* WRAPPER_HPP */

