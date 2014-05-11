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

#if defined( __cplusplus ) && __cplusplus > 

#endif	/* WARNING_HPP */

