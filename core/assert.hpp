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

#ifndef ASSERT_HPP
#define	ASSERT_HPP

#ifdef USE_CPP_TRICKS

//Thanks to the lack of macro optional parameters, we should do this freaking tricks...:

#define TURBO_ASSERT_IMPL_DEFAULT_MESSAGE( condition )        static_assert( condition() , "assertion failed" )
#define TURBO_ASSERT_IMPL_USER_MESSAGE( condition , message ) static_assert( condition() , message )

#define CHOOSE_THIRD_ARG(first , second , third , ...) third
#define CHOOSE_TURBO_ASSERT_OVERLOAD(...) CHOOSE_THIRD_ARG(__VA_ARGS__ , TURBO_ASSERT_IMPL_USER_MESSAGE , TURBO_ASSERT_IMPL_DEFAULT_MESSAGE )

#define turbo_assert(...) CHOOSE_TURBO_ASSERT_OVERLOAD(__VA_ARGS__)(__VA_ARGS__)

#else

#define turbo_assert( condition ) static_assert( condition::value , "assertion failed" )

#endif  /* USE_CPP */
#endif	/* ASSERT_HPP */

