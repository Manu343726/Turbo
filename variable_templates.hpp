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

#ifndef VARIABLE_TEMPLATES_HPP
#define	VARIABLE_TEMPLATES_HPP

/* C++11-compliant pseudo C++1y variable templates */

#define TURBO_INDEXED_VARIABLE( type , name ) \
    template<int id>                          \
    type&  name()                             \
    {                                         \
        static type _variable_ = type{};      \
        return _variable_;                    \
    }
        
#define TURBO_MUTABLE_VARIABLE( name ) \
    template<typename T>               \
    T&  name()                         \
    {                                  \
        static T _variable_ = T{};     \
        return _variable_;             \
    }

#endif	/* VARIABLE_TEMPLATES_HPP */

