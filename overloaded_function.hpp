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
#include "algorithm.hpp"
#include "placeholders.hpp"

using namespace tml::placeholders;

namespace tml
{   
    namespace runtime
    {
        template<typename F , typename... Fs>
        struct overloaded_function : public tml::std_function<F> , public tml::std_function<Fs>...
        {
            overloaded_function( F f , Fs... fs ) :
                tml::std_function<F>{ f },
                tml::std_function<Fs>{ fs }...
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
        
        template<typename F , typename... Fs>
        struct function_signature<tml::runtime::overloaded_function<F,Fs...>>
        {
            using return_type = tml::list<tml::function_return_type<F>,tml::function_return_type<Fs>...>;
            using args        = tml::list<tml::function_argumments<F>,tml::function_argumments<Fs>...>;
        };
        
        template<typename F , typename... Fs , typename... ARGS>
        struct is_valid_call<tml::runtime::overloaded_function<F,Fs...>,ARGS...> :
            public tml::function<tml::any<tml::bind<tml::func::is_valid_call,_1,ARGS...>,tml::list<F,Fs...>>>
        {};
    }
}

#endif /* OVERLOADED_FUNCTION_HPP */

