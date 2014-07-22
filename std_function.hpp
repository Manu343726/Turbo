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

#ifndef STD_FUNCTION_HPP
#define	STD_FUNCTION_HPP

#include "type_traits.hpp"
#include "list.hpp"
#include "function.hpp"

/*
 * The metafunction tml::std_function returns an equivalent std::function instance given
 * a function entity type F. For example:
 * 
 *     auto f = [](int){};
 *     tml::std_function<decltype(f)> f_wrapper{ f };
 * 
 * Note that equivalent means that the returned std::function has the same signature of
 * the passed entity.
 */

namespace tml
{
    namespace impl
    {
        template<typename R , typename ARGS>
        struct std_function_builder;
        
        template<typename R , typename... ARGS>
        struct std_function_builder<R,tml::list<ARGS...>> : public tml::function<std::function<R(ARGS...)>>
        {};
        
        template<typename F>
        struct std_function : 
            public tml::impl::std_function_builder<tml::function_return_type<F>,tml::function_argumments<F>>
        {};
    }
    
    namespace func
    {
        /*
         * Given a function entity with type F, returns a std::function with the same signature.
         */
        template<typename F>
        using std_function = tml::impl::std_function<F>;
    }
    
    /*
     * Given a function entity with type F, returns a std::function with the same signature.
     */
    template<typename F>
    using std_function = tml::eval<tml::func::std_function<F>>;
    
    namespace runtime
    {
        /*
         * Given a function entity f, returns a std::function object wrapping f.
         */
        template<typename F>
        tml::std_function<F> make_function( F f )
        {
            return { f };
        }
    }
}

#endif	/* STD_FUNCTION_HPP */

