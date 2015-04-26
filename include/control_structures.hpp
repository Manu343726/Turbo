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

#ifndef CONTROL_STRUCTURES_HPP
#define	CONTROL_STRUCTURES_HPP

#include <type_traits>

#include "eval.hpp"
#include "lazy.hpp"

/*
 * This header declares simple imperative-programming control structures, like ifs, switches, etc
 */

namespace tml
{
    /*
     * Selects one type or other based on a boolean condition.
     * Its the equivalent of the if control structure.
     * 
     * The parameters are:
     *  - CONDITION: A boolean expression representing the condition of the if.
     *  - TRUE: Type which will be selected if the evaluation of the condition returns true.
     *  - FALSE: Type which will be selected if the evaluation of the condition returns false.
     */
    template<typename CONDITION , typename TRUE , typename FALSE>
    using conditional = typename std::conditional<tml::eval<CONDITION>::value , TRUE , FALSE>::type;
    
    namespace impl
    {
        /*
         * std::conditional equivalent for let expressions contexts
         */
        template<typename CONDITION , typename TRUE , typename FALSE>
        struct dconditional : public tml::function<TRUE>
        {};

        template<typename TRUE , typename FALSE>
        struct dconditional<tml::false_type,TRUE,FALSE> : public tml::function<FALSE>
        {};
    }
    
    /*
     * Selects one type or other based on a boolean condition on let expression contexts.
     * The evaluation of the condition is delayed until variable substitution.
     * 
     * Its the equivalent of the if control structure
     * 
     * The parameters are:
     *  - CONDITION: A boolean expression representing the condition of the if.
     *  - TRUE: Type which will be selected if the evaluation of the condition returns true.
     *  - FALSE: Type which will be selected if the evaluation of the condition returns false.
     * 
     * Note that this is a function, not an alias directly, so it should be evaluated to get the 
     * result.
     */
    template<typename CONDITION , typename TRUE , typename FALSE>
    using dconditional = tml::impl::dconditional<tml::deval<CONDITION> , TRUE , FALSE>; 
    
    
    /*
     * Selects one type or other based on a boolean condition, where the passed types
     * could be lazy instantiated templates.
     * This allows to select between potentially ill-formed types conditionally.
     * 
     * Its the equivalent of the if control structure
     * 
     * The parameters are:
     *  - CONDITION: A boolean expression representing the condition of the if.
     *  - TRUE: Type which will be selected if the evaluation of the condition returns true.
     *  - FALSE: Type which will be selected if the evaluation of the condition returns false.
     */
    template<typename CONDITION , typename TRUE , typename FALSE>
    using lconditional = tml::lazy_instance<tml::conditional<CONDITION,TRUE,FALSE>>;
}

#endif	/* CONTROL_STRUCTURES_HPP */

