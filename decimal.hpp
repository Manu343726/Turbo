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

#ifndef DECIMAL_HPP
#define	DECIMAL_HPP

#include "fixed_point.hpp"
#include "math.hpp"


/*
 * This header defines metafunctions for instancing fixed-point values (tml::fixed_point) using decimal
 * scientific notation.
 */

namespace tml
{
    namespace impl
    {
        template<typename INTEGER_T , INTEGER_T mantissa , INTEGER_T exponent>
        struct decimal : public tml::function<tml::eval<tml::mul<tml::pow<tml::adjusted_fixed_point<INTEGER_T,10>,
                                                                          tml::integral_constant<INTEGER_T,exponent>
                                                                         >,
                                                                 tml::adjusted_fixed_point<INTEGER_T,mantissa>
                                                                >
                                                       >
                                             >
        {};
    }
    
    /*
     * Specifies a fixed-point value with underlying integer type INTEGER_T
     * using decimal scientific notation.
     */
    template<typename INTEGER_T , INTEGER_T mantissa , INTEGER_T exponent>
    using decimal = typename tml::impl::decimal<INTEGER_T,mantissa,exponent>::result;
    
    /*
     * Specifies a tml::fsingle value with underlying integer type INTEGER_T
     * using decimal scientific notation.
     */
    template<tml::impl::single_t mantissa , tml::impl::single_t exponent>
    using decimal_fsingle = tml::decimal<tml::impl::single_t,mantissa,exponent>;
    
    /*
     * Specifies a tml::fdouble value with underlying integer type INTEGER_T
     * using decimal scientific notation.
     */
    template<tml::impl::double_t mantissa , tml::impl::double_t exponent>
    using decimal_fdouble = tml::decimal<tml::impl::double_t,mantissa,exponent>;
}

#endif	/* DECIMAL_HPP */

