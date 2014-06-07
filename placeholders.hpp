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

#ifndef PLACEHOLDERS_HPP
#define	PLACEHOLDERS_HPP

#include "basic_types.hpp"
#include "eval.hpp"
#include "function.hpp"
#include "warning.hpp"
#include "to_string.hpp"
#include "chameleon.hpp"

#include <string>

namespace tml
{
    struct placeholder_val {};
    
    /*
     * This template is used to identify a placeholder.
     * A placeholder should behave like values and functions to work inside
     * some evaluation contexts, so they use chameleons to mimic that primitives
     * (See documentation at "chameleon.hpp" for more details)
     */
    template<std::size_t INDEX>
    struct placeholder : public tml::value_chameleon , public tml::function_chameleon
    {};
    
    /*
     * Placeholders don't represent values, but they should be correctly evaluable,
     * because they are used to build expressions which are evaluable (A lambda expression
     * for example). 
     *
     * For that purpose, the placeholders override the default behaviour of tml::eval.
     * The result of evaluating a placeholder is tml::placeholders::placeholder_val
     */
    template<std::size_t I>
    struct overrides_eval<tml::placeholder<I>> : public tml::true_type
    {
        TURBO_WARNING( (tml::Bool<sizeof(I) != sizeof(I)>) , "overrides_eval of placeholders instanced" );
    };

    namespace impl
    {
        template<std::size_t I , typename... ARGS>
        struct eval<tml::placeholder<I>,tml::list<ARGS...>> : public tml::function<tml::placeholder<I>>
        {};
        
        /*
         * to_string override for placeholders
         */
        template<std::size_t INDEX>
        struct to_string<tml::placeholder<INDEX>>
        {
            operator std::string() const
            {
                return "_" + std::to_string( INDEX );
            }
        };
    }
        
    /*
     * Set of placeholders which could be used in function-parameters binding expressions.
     * 
     * See tml::eval documentation in 'functional.hpp' for more information.
     */
    namespace placeholders
    {   
        using _  = tml::placeholder<std::numeric_limits<std::size_t>::max()>;
        using _1 = tml::placeholder<1>;
        using _2 = tml::placeholder<2>;
        using _3 = tml::placeholder<3>;
        using _4 = tml::placeholder<4>;
        using _5 = tml::placeholder<5>;
        using _6 = tml::placeholder<6>;
        using _7 = tml::placeholder<7>;
        using _8 = tml::placeholder<8>;
        using _9 = tml::placeholder<9>;
        using _10 = tml::placeholder<10>;
        using _11 = tml::placeholder<11>;
        using _12 = tml::placeholder<12>;
        using _13 = tml::placeholder<13>;
        using _14 = tml::placeholder<14>;
        using _15 = tml::placeholder<15>;
        using _16 = tml::placeholder<16>;
    }
}

#endif	/* PLACEHOLDERS_HPP */

