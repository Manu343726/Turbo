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

#ifndef STATIC_IF_HPP
#define	STATIC_IF_HPP

#include "impl/CPP_META_MACROS.hpp"
#include "basic_types.hpp"
#include "lazy.hpp"
#include "control_structures.hpp"

#define IF_BODY(name , ...) template<typename DUMMY>         \
                            struct CAT_TOKENS(if_body_,name) \
                            {                                \
                               struct return_type{};         \
                               static return_type run()      \
                               {                             \
                                   [&]()__VA_ARGS__();       \
                                   return return_type{};     \
                               }                             \
                            };      

#define THEN , (
#define ELSE ) , (
#define END_IF )
#define STATIC_IF_IMPL( id , cond , true_ , false_ )  IF_BODY( CAT_TOKENS( true_case_  , id ) , STRIP_PARENS true_ )           \
                                                      IF_BODY( CAT_TOKENS( false_case_ , id ) , STRIP_PARENS false_ )          \
auto CAT_TOKENS(if_variable_ , id) = tml::lazy_instance<tml::conditional<STRIP_PARENS cond ,                                   \
                                                                         tml::lazy<CAT_TOKENS(if_body_true_case_  , id),void>, \
                                                                         tml::lazy<CAT_TOKENS(if_body_false_case_ , id),void>  \
                                                                        >                                                      \
                                                       >::run();    

#define STATIC_IF( cond , true_ , false_ ) STATIC_IF_IMPL( EXPAND_MACRO(__LINE__) , cond , true_ , false_ )

#define IF(...) STATIC_IF(__VA_ARGS__)

#endif	/* STATIC_IF_HPP */

