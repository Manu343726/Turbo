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

#ifndef FUNCTION_ALIAS_DECL_HPP
#define	FUNCTION_ALIAS_DECL_HPP

/*
 * This macro defines the user-side template alias for a specified function.
 * 
 * The macro assumes the function follows the Turbo function implementation convention:
 *  - Given a function name "f", a template alias (Which aliases to the result of the
 *    function) is declared in the tml namespace.
 *  - The implementation is defined in a private namespace named impl, nested into the
 *    user-alias namespace (tml in this case, so the implementation namespace is tml::impl).
 *  - Only type template argumments are used.
 */
#define TURBO_DEFINE_FUNCTION_TALIAS( function ) \
        template<typename... ARGS>               \
        using  function = typename tml::impl::function<ARGS...>::result; 

/*
 * This macro defines the user-side template alias for a specified function, but
 * a custom name for the alias is used. This macro is dessigned to implement different
 * calling conventions for the same function.
 * 
 * The macro assumes the function follows the Turbo function implementation convention:
 *  - Given a function name "f", a template alias (Which aliases to the result of the
 *    function) is declared in the tml namespace.
 *  - The implementation is defined in a private namespace named impl, nested into the
 *    user-alias namespace (tml in this case, so the implementation namespace is tml::impl).
 *  - Only type template argumments are used.
 */
#define TURBO_DEFINE_FUNCTION_TALIAS_CUSTOMNAME( function , name ) \
        template<typename... ARGS>               \
        using  name = typename tml::impl::function<ARGS...>::result; 

#endif	/* FUNCTION_ALIAS_DECL_HPP */

