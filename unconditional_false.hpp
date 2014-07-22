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

#ifndef ASSERTING_CONDITIONS_HPP
#define	ASSERTING_CONDITIONS_HPP

#include "basic_types.hpp"


/*
 * Normally is desirable that asserting conditions on template will be evaluated at instantation phase, 
 * not at declaration phase, because the assert or warning will be throwed even if the template is not
 * instantiated (Used).
 * The common way to delay the evaluation of such asserting expressions is to make them depend on
 * template parameters.
 * 
 * The following definitions are templates wich represent unconditional false boolean expressions.
 * That expressions could be used to assert or raise a warning unconditionally, but they will be evaluated
 * instantation phase.
 * 
 * An example:
 * 
 *     template<typename T>
 *     struct foo
 *     {
 *         TURBO_WARNING( (tml::unconditional_false<T>)  , "Be carefull, you are using the foo template" );
 *     }
 */

namespace tml
{
    /*
     * This template represents a boolean false condition which evaluation is delayed until instantation time
     * via a template parameter or a variadic pack.
     */
    template<typename... ARGS>
    struct unconditional_false
    {
        static constexpr const bool value = sizeof...(ARGS) != sizeof...(ARGS);
    };
}

#endif	/* ASSERTING_CONDITIONS_HPP */

