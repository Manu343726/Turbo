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

#ifndef RAII_ACTIONS_HPP
#define	RAII_ACTIONS_HPP

#include <type_traits>

namespace tml
{
    namespace runtime
    {
        struct scope_begin_action
        {
            template<typename F>
            scope_begin_action( F&& f )
            {
                f();
            }
        };
        
        template<typename F>
        struct scope_end_action
        {
            scope_end_action( F&& f ) : _action{ std::move( f ) }
            {}
            
            ~scope_end_action()
            {
                _action();
            }
                    
        private:
            F _action;
        };
        
        template<typename E>
        struct scope_actions : public tml::runtime::scope_begin_action , public tml::runtime::scope_end_action<E>
        {
            template<typename B , typename EE>
            scope_actions( B&& begin , EE&& end ) : 
                tml::runtime::scope_begin_action{ std::forward<B>( begin ) } , 
                tml::runtime::scope_end_action<E>{ std::forward<EE>( end ) }
            {}
        };
        
        template<typename B>
        tml::runtime::scope_begin_action on_scope_begin( B&& begin )
        {
            return { std::forward<B>( begin ) };
        }
        
        template<typename E>
        tml::runtime::scope_end_action<typename std::decay<E>::type> on_scope_end( E&& end )
        {
            return { std::forward<E>( end ) };
        }
        
        template<typename B , typename E>
        tml::runtime::scope_actions<typename std::decay<E>::type> on_scope( B&& begin , E&& end )
        {
            return { std::forward<B>( begin ) , std::forward<E>( end ) };
        }
    }
}

#endif	/* RAII_ACTIONS_HPP */

