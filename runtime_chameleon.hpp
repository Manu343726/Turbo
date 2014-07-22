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

#ifndef RUNTIME_CHAMELEON_HPP
#define	RUNTIME_CHAMELEON_HPP

#include <ostream>
#include <istream>

/*
 * The type tml::runtime::chameleon is dessigned to give a working 
 * type for any context. This could be usefull when creating tag types
 * which should work like the rest of types.
 */

namespace tml
{
    namespace runtime
    {
        struct chameleon
        {
            /*
             * The chameleon is implicitly convertible to any default-constructible
             * type.
             */
            template<typename T>
            constexpr operator T() const
            {
                return T{};
            }
            
            constexpr chameleon() = default;
            
            /*
             * The chamaleon overloads IO stream operators to be usable in such cotexts.
             */
            
            friend std::ostream& operator<<( std::ostream& os , const chameleon& )
            {
                return os;
            }
            
            friend std::istream& operator>>( std::istream& is , chameleon& )
            {
                return is;
            }
        };
    }
}

#endif	/* RUNTIME_CHAMELEON_HPP */

