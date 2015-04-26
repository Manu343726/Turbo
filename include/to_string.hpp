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

#ifndef TO_STRING_HPP
#define	TO_STRING_HPP

/*
 * This header declares a set of utility functions and metafunctions to get string representations
 * of metaprogramming values at runtime.
 * 
 * For this purpose, Turbo defines the template function tml::to_string<T>(), which returns a string
 * representation of a type T. The function is overloaded to allow the suer to explicitly specify the type
 * via template argumment, or rely on template argumment deduction of the function argumment.
 * 
 * For example:
 * 
 *     struct foo{};
 * 
 *     int main()
 *     {
 *         foo myfoo;
 * 
 *         std::cout << tml::to_string<foo>() << std::endl;
 *         std::cout << tml::to_string( myfoo ) << std::endl;
 *     }
 * 
 * The (runtime) function tml::to_string() uses the metafunction tml::impl::to_string to compute the name string.
 * This metafunction overloads the std::string conversion operator to retrieve the string when instancing (Runtime 
 * instance, not template instance) the type on contexts expecting std::string (Specifically, the return statement of 
 * tml::to_string() function).
 * 
 * By default tml::impl::to_string computes the string via RTTI and applies demangling (If supported) on the result.
 * The user could customize the string representation of his own types specializing tml::impl::to_string for them. 
 */

#include <string>

#include "manu343726/turbo_core/demangle.hpp"

namespace tml
{
    namespace impl
    {
        /*
         * Default implementation of to_string metafunction
         */
        template<typename T>
        struct to_string
        {
            operator std::string() const
            {
                return tml::impl::demangle( typeid( T ).name() );
            }
        };
    }
    
    template<typename T>
    std::string to_string()
    {
        return tml::impl::to_string<T>{}; //Instances tml::impl::to_string and calls conversion op.
    }
    
    template<typename T>
    std::string to_string( const T& )
    {
        return tml::to_string<T>();
    }
}

#endif	/* TO_STRING_HPP */


