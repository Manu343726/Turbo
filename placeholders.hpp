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

namespace tml
{
    /*
     * Set of placeholders which could be used in function-parameters binding expressions.
     * 
     * See tml::eval documentation in 'functional.hpp' for more information.
     */
    namespace placeholders
    {
        
        struct placeholder_val {};
        
        /*
         * This template is used to identify a placeholder.
         * The result of evaluating a placeholder, and an expression containing placeholders
         * is tml::placeholders::placeholder_val
         */
        template<std::size_t INDEX>
        struct placeholder
        {
            /* 
             * Several memers are defined to make the instantation of functions using placeholders
             * work in almost all cases.
             * 
             * The most common cases in the Turbo library where members of tyes are accessed are basic 
             * values (Their 'value' static member constant) and functions (Their 'result' member type).
             */
            using result = tml::placeholders::placeholder_val;
            static constexpr const std::size_t value = -1;
        };
        
        
        struct _1 : public placeholder<1>{};
        struct _2 : public placeholder<2>{};
        struct _3 : public placeholder<3>{};
        struct _4 : public placeholder<4>{};
        struct _5 : public placeholder<5>{};
        struct _6 : public placeholder<6>{};
        struct _7 : public placeholder<7>{};
        struct _8 : public placeholder<8>{};
        struct _9 : public placeholder<9>{};
        struct _10 : public placeholder<10>{};
        struct _11 : public placeholder<11>{};
        struct _12 : public placeholder<12>{};
        struct _13 : public placeholder<13>{};
        struct _14 : public placeholder<14>{};
        struct _15 : public placeholder<15>{};
        struct _16 : public placeholder<16>{};
    }

    /*
     * Placeholders don't represent values, but they should be correctly evaluable,
     * because they are used to build expressions which are evaluable (A lambda expression
     * for example). 
     *
     * For that purpose, the placeholders override the default behaviour of tml::eval.
     * The result of evaluating a placeholder is tml::placeholders::placeholder_val
     */
    template<std::size_t I>
    struct overrides_eval<tml::placeholders::placeholder<I>> : public tml::true_type
    {
        TURBO_WARNING( (tml::Bool<sizeof(I) != sizeof(I)>) , "overrides_eval of placeholders instanced" );
    };

    namespace impl
    {
        template<std::size_t I , typename... ARGS>
        struct eval<tml::placeholders::placeholder<I>,tml::list<ARGS...>> : public tml::function<tml::placeholders::placeholder_val>
        {};
    }
}

#endif	/* PLACEHOLDERS_HPP */

