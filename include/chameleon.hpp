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

#ifndef CHAMELEON_HPP
#define	CHAMELEON_HPP

/*
 * There are situations (Commonly pre-evaluation contexts) where certain
 * types should behave like Turbo primitives (i.e. values and functions).
 * One example are placeholders, which would be used in situations where 
 * the user expects a ::value member type, even if a placeholder is not 
 * a value itself:
 * 
 *     template<typename T>
 *     struct odd : public tml::function<tml::Bool<T::value % 2 == 0>>
 *     {};
 * 
 *     using is_odd = tml::lambda<_1 , odd<_1>>; 
 * 
 * This header defines a set of so called "chameleon types", designed to 
 * mimic the interface of some Turbo primitives. 
 */

namespace tml
{
    /*
     * Mimics the interface of a Turbo integral value primitive
     */
    struct value_chameleon
    {
        static constexpr const int value = 0;
    };
    
    /*
     * Mimics the interface of a Turbo function primitive
     */
    struct function_chameleon
    {
        struct result : public tml::value_chameleon {};
    };
    
    /*
     * Acts as a function which evaluation should work on contexts expecting values.
     * Also should behave as a function, because its a function not evaluated yet.
     */
    struct not_evaluated_function_chameleon : public tml::value_chameleon , public tml::function_chameleon
    {};
}


#endif	/* CHAMELEON_HPP */

