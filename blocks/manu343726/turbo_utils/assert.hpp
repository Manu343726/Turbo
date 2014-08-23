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

#include "manu343726/turbo_core/cpp_meta_macros.hpp"

/*
 * Evaluates a template boolean expression and aborts compilation if the 
 * result of the evaluation is false.
 * 
 * C++11 static_assert() compile-time assert utility expects a boolean value as
 * asserting condition, allowing us to abort the compilation depending on the result
 * of evaluating a condition at compile time. Also allows us to provide a useful message
 * for assert fail. For example:
 * 
 *     static_assert( sizeof(char) == 1 , "Non standard-complaint compiler" );
 * 
 * As explained above, static_assert() expect a compile-time bool value. That value
 * could be any kind of compile-time boolean expression, templates included:
 * 
 *     static_assert( std::is_pointer<int*>::value , "What?" );
 * 
 * That kind of compile-time assertion based on templates is extremely useful
 * (Consider concept checking based on type traits, for example), but for complex
 * template expressions, like that which this template metaprogramming library encourages,
 * the syntax "[template-instance]::value" could be a bit verbose.
 * Also, this library encourages the use of types as values prior to naked values, which
 * make template expressions uniform (No value parameters, all parameters are types).
 * 
 * The macro TURBO_ASSERT() wraps C++11 static_assert() allowing us to pass a template boolean
 * expression as argumment directly, without writting ::value. The macro has two parameters:
 * 
 *  - CONDITION: The asserting condition, expressed as a template boolean expression.
 *               Like every macro expecting a template parameter, the CONDITION template parameter
 *               should be enclosed with parens.
 *               See the documentation of STRIP_PARENS() macro on CPP_META_MACROS.hpp for more info.
 * 
 *  - MESSAGE: Debugging message which will be shown when the assertion fails. Its default value
 *             is "assertion failed".
 */

/* Default overload: No debugging message specified by the user, default value used instead.*/
#define TURBO_ASSERT_IMPL_DEFAULT_MESSAGE( condition )        static_assert( STRIP_PARENS condition :: value , "assertion failed" )
/* Second overload: A debugging message was specified by the user.*/
#define TURBO_ASSERT_IMPL_USER_MESSAGE( condition , message ) static_assert( STRIP_PARENS condition :: value , message )

/* This macro selects the proper overload depending on the number of argumments passed by the user. */
#define CHOOSE_TURBO_ASSERT_OVERLOAD(...) SELECT_ARG_3(__VA_ARGS__ , TURBO_ASSERT_IMPL_USER_MESSAGE , TURBO_ASSERT_IMPL_DEFAULT_MESSAGE )

/* TURBO_ASSERT() macro: Forward the parameters to the overload chooser and then to the selected overload. */
#define TURBO_ASSERT(...) CHOOSE_TURBO_ASSERT_OVERLOAD(__VA_ARGS__)(__VA_ARGS__)


#endif	/* ASSERT_HPP */

