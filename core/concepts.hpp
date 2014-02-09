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

#ifndef ARITHMETIC_CONCEPTS_HPP
#define	ARITHMETIC_CONCEPTS_HPP

#include "boolean_predicates.hpp"

/*
 * Arithmetic type predicate traits. 
 * 
 * A predicate trait is a special case of a type trait which acts as a boolean predicate which evaluates/checks a certain property of the type specified.
 * Default traits are defined as false boolean predicates, and optionally, with an error message (static_assert). User should register new types to fulfill a concept (Specialize the trait to true).
 * The library provides a set of macros to automatize the trait definition process.
 */



//Macro to automatize the definition of unary trait predicates. Its defined as an evaluable boolean predicate (Defined as false by default)
#define make_unary_trait( trait_name ) \
template<typename T>                   \
struct trait_name : public tml::false_predicate {}




//This macro defines an asserting predicate trait, that is, a predicate trait which contains an assertion dependent on the trait itself.
//If the predicate trait evaluates to false, the assertion fails and shows the specified error message.

#define make_unary_trait_assert( trait_name , error_message ) \
template<typename T>                                          \
struct assert_##trait_name : public trait_name                \
{                                                             \
    //Make the assertion condition template dependent         \
    static_assert( trait_name<T>::value , error_message )     \
}

#define make_complete_unary_trait( trait_name , error_message ) make_unary_trait( trait_name ); make_unary_trait_assert( trait_name , error_message )

#endif	/* ARITHMETIC_CONCEPTS_HPP */

