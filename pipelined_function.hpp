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

#ifndef POLICIED_FUNCTION_HPP
#define	POLICIED_FUNCTION_HPP

#include "type_traits.hpp"
#include "curry.hpp"
#include "bind.hpp"
#include "raii_actions.hpp"

#include <functional>


namespace tml
{
    namespace runtime
    {
        template<typename S>
        struct pipelined_function
        {
            template<typename... ARGS>
            tml::function_return_type<S> operator()( ARGS&&... args )
            {
                auto guard = tml::runtime::on_scope_end( std::bind( _on_end , args... ) );
                
                _on_begin( std::forward<ARGS>( args )... );
                return _body( std::forward<ARGS>( args )... );    
            }
            
            template<typename F>
            pipelined_function& begin( F f )
            {
                _on_begin = f;
                return *this;
            }
            
            template<typename F>
            pipelined_function& body( F f )
            {
                _body = f;
                return *this;
            }
            
            template<typename F>
            pipelined_function& end( F f )
            {
                _on_end = f;
                return *this;
            }
            
        private:
            using begin_t = tml::eval<tml::euncurry<tml::lazy<tml::func::make_function_signature,void>>,tml::function_argumments<S>>;
            using end_t = begin_t;
            using body_t = S;
            
            std::function<begin_t> _on_begin;
            std::function<end_t>   _on_end;
            std::function<body_t>  _body;
        };
    }
}

#endif	/* POLICIED_FUNCTION_HPP */

