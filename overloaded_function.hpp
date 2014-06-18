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

#ifndef OVERLOADED_FUNCTION_HPP
#define	OVERLOADED_FUNCTION_HPP

#include <functional>
#include <type_traits>

#include "eval.hpp"
#include "type_traits.hpp"
#include "std_function.hpp"

namespace tml
{   
    namespace runtime
    {
        template<typename F , typename... Fs>
        struct overloaded_function : public tml::std_function<F> , public tml::std_function<Fs>...
        {
            overloaded_function( F f , Fs... fs ) :
                F{ f },
                Fs{ fs }...
            {}
        };


        template<typename F , typename... Fs>
        tml::runtime::overloaded_function<F,Fs...> make_overloaded_function( F f , Fs... fs )
        {
            return { f , fs... };
        }
    }
}

namespace tml
{
    namespace impl
    {
        template<typename F , typename... Fs>
        struct is_functor<tml::runtime::overloaded_function<F,Fs...>> : public tml::function<tml::true_type>
        {};
        
        template<typename... Fs>
        struct function_signature<tml::runtime::overloaded_function<Fs...>>
        {
            using return_type = tml::list<tml::function_return_type<Fs>...>;
            using args        = tml::list<tml::function_argumments<Fs>...>;
        };
    }
}

