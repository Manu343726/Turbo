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

#ifndef WARNING_HPP
#define	WARNING_HPP

/*
 * Portable static_warning utility.
 * 
 * The macro TURBO_STATIC_WARNING() works exactly as TURBO_STATIC_ASSERT(), but it emmits compilation warnings
 * instead of aborting compilation when the condition is false. Like assert, warning expects a boolean value type
 * as parameter.
 * 
 * The implementation works enabling the deprecated attribute if the condition is false. The implementation checks for
 * C++14 support (Which has a standard [[deprecated(message)]] attribute), else a compiler-specific version is used.
 */

#include "basic_types.hpp"
#include "impl/CPP_META_MACROS.hpp"

#define CPP11_TAG 201103
#define CPP_ENABLED defined( __cplusplus )
#define CPP11_ENABLED CPP && __cplusplus == CPP11_TAG
#define CPP1Y_ENABLED CPP && __cplusplus > CPP11_TAG

#if CPP1Y_ENABLED
#define DEPRECATED_ATTRIBUTE(x,message) [[deprecated(message)]] x
#elif __GNUC__

#elif __llvm__ 
#define DEPRECATED_ATTRIBUTE(x,message) x __attribute__((deprecated(message)))
#else
#error "Unsupported compiler"
#endif

#define TURBO_WARNING( condition , message )                \
    struct UNIQUE_IDENTIFIER_LINE( turbo_static_warning )   \
    {                                                       \
        UNIQUE_IDENTIFIER_LINE( turbo_static_warning )()    \
        {                                                   \
            warning_call( STRIP_PARENS condition {} );      \
        }                                                   \
                                                                                \
        DEPRECATED_ATTRIBUTE( void warning_call( tml::false_type ) , "[[STATIC_WARNING]] " message " [[STATIC_WARNING]]" ){} \
        void warning_call( tml::true_type ){}                                  \
    }                                        

#endif	/* WARNING_HPP */

