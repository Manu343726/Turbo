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

#ifndef RUNTIME_PLACEHOLDERS_HPP
#define	RUNTIME_PLACEHOLDERS_HPP

#include "manu343726/turbo_core/runtime_chameleon.hpp"

/*
 * These placeholders are placeholders designed to represent compile-time values 
 * instead of types. They are usable in any context (Runtime or compile-time) where 
 * a value is expected. 
 */

namespace tml
{
    namespace runtime
    {
        template<std::size_t I>
        struct placeholder : public tml::runtime::chameleon
        {
            constexpr placeholder() {};
        };
            
        namespace placeholders
        {
            constexpr const tml::runtime::placeholder<std::numeric_limits<std::size_t>::max()> __;
            constexpr const tml::runtime::placeholder<0>  __1;
            constexpr const tml::runtime::placeholder<1>  __2;
            constexpr const tml::runtime::placeholder<2>  __3;
            constexpr const tml::runtime::placeholder<3>  __4;
            constexpr const tml::runtime::placeholder<4>  __5;
            constexpr const tml::runtime::placeholder<5>  __6;
            constexpr const tml::runtime::placeholder<6>  __7;
            constexpr const tml::runtime::placeholder<7>  __8;
            constexpr const tml::runtime::placeholder<8>  __9;
            constexpr const tml::runtime::placeholder<9>  __10;
            constexpr const tml::runtime::placeholder<10> __11;
            constexpr const tml::runtime::placeholder<11> __12;
            constexpr const tml::runtime::placeholder<12> __13;
            constexpr const tml::runtime::placeholder<13> __14;
            constexpr const tml::runtime::placeholder<14> __15;
            constexpr const tml::runtime::placeholder<15> __16;
        }
    }
}

#endif	/* RUNTIME_PLACEHOLDERS_HPP */

