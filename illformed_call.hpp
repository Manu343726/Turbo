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

#ifndef ALGORITHM_POLICIES_HPP
#define	ALGORITHM_POLICIES_HPP

#include "eval.hpp"
#include "enable_if.hpp"
#include "function.hpp"
#include "type_traits.hpp"
#include "std_function.hpp"
#include "runtime_chameleon.hpp"

#include <utility>
#include <type_traits>
#include <functional>

namespace tml
{
    namespace runtime
    {
        /*
         * The functor tml::runtime::illformed_call represents a function entity which
         * supports ill-formed function calls to itself.
         * 
         * Suppose we have a function f which takes two ints:
         * 
         *     void f( int , int )
         *     { 
         *         ... 
         *     }
         * 
         * The call f(1,2) is well-formed (Syntactically/semantically correct), but
         * the call f("hello") is not.
         * 
         * 'illformed_call' supports such ill-formed calls, making them compilable.
         * That functor wraps a function entity f, and when you call the functor:
         * 
         *    ill( "hello" );
         * 
         * If the call is well-formed it calls the wrapped function, and if is ill-formed performs
         * an action specified by the user. For example:
         * 
         *     auto f = tml::runtime::make_illformed_call( []( std::vector<int>& v ){ v.push_back( 1 ); } ,
         *                                                 [](){ throw std::exception{ "Thats not a vector!!!" }; }
         *                                               );
         * 
         *     f();
         * 
         * In this example, the call to f throws an exception because the call is ill-formed.
         */
        template<typename F , typename E>
        struct illformed_call
        {
            illformed_call( F f , E else_ ) :
                _f{ f } ,
                _else{ else_ }
            {}

            struct void_return : public tml::runtime::chameleon
            {};

            template<typename... ARGS , typename FF = F , typename = TURBO_ENABLE_IF( tml::is_valid_call<FF,ARGS...> )>
            tml::function_return_type<F> operator()( ARGS&&... args ) const
            {
                return _f( std::forward<ARGS>( args )... );
            }

            template<typename... ARGS , typename FF = F , typename EE = E , 
                     typename = TURBO_SFINAE_ALL( DISABLE_IF( tml::is_valid_call<FF,ARGS...> ),
                                                  DISABLE_IF( std::is_same<void,tml::function_return_type<EE>> )
                                                )
                    >
            tml::function_return_type<E> operator()( ARGS&&... args ) const
            {
                return _else();
            }

            template<typename... ARGS , typename FF = F , typename = TURBO_SFINAE_ALL( DISABLE_IF( tml::is_valid_call<FF,ARGS...> ),
                                                                                       ENABLE_IF( std::is_same<void,tml::function_return_type<E>> )
                                                                                     )
                    >
            void_return operator()( ARGS&&... args ) const
            {
                _else();
                return void_return{};
            }

        private:
            tml::std_function<F> _f;
            tml::std_function<E> _else;
        };

        template<typename F , typename E>
        tml::runtime::illformed_call<F,E> make_illformed_call( F f , E else_ )
        {
            return { f , else_ };
        }
    }
}

#endif	/* ALGORITHM_POLICIES_HPP */

