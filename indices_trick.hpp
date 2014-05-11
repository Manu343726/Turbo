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

#ifndef INDICES_TRICK_HPP
#define	INDICES_TRICK_HPP

#include "basic_types.hpp"
#include "integral_lists.hpp"


/*
 * This header defines the set of utilities (Integral sequences, variadic-pack expansion, etc) neccesary to
 * implement easily the indices trick.
 */

namespace tml
{
    namespace itrick
    {
        namespace impl
        {
            /*
             * Generates a sequence of consecutive indices ('tml::size_t's) in the form 0,1,...,LENGTH-1
             */
            template<typename LENGTH , typename INDICES>
            struct sequence_generator;
            
            /*
             * Recursive case
             */
            template<std::size_t LENGTH , typename... INDICES>
            struct sequence_generator<tml::size_tt<LENGTH>,tml::list<INDICES...>> :
                public sequence_generator<tml::size_tt<LENGTH-1>,tml::list<tml::size_tt<LENGTH>,INDICES...>>
            {};
            
            /*
             * Base case:
             */
            template<typename... INDICES>
            struct sequence_generator<tml::size_tt<0>,tml::list<INDICES...>> : public tml::function<tml::list<tml::size_tt<0>,INDICES...>>
            {};
        }
        
        /*
         * Generates a sequence of consecutive indices ('tml::size_t's) in the form 0,1,...,SIZE-1
         */
        template<typename SIZE>
        using indices_sequence = typename tml::itrick::impl::sequence_generator<SIZE,tml::empty_list>::result;
        
    }
}

#endif	/* INDICES_TRICK_HPP */

