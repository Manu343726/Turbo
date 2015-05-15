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

#ifndef MAP_HPP
#define	MAP_HPP

#include "integral.hpp"

#include <type_traits>


/**
 * This header provides a tml::map template designed for fast associetive searching of types.
 */


namespace tml
{
    
    
    namespace maps
    {
        /**
         * Represents an entry of a map, as a pair (key,value)
         */
        template<typename KEY , typename VALUE>
        struct pair
        {
            using key = KEY;
            using value = VALUE;
        };

        template<typename P> using get_key = typename P::key;
        template<typename P> using get_value = typename P::value;
        
        
        struct map_base
        {
            struct not_found{};
            
            template<typename K> not_found test(...);
        };

        template<typename... PAIRS>
        struct map
        {
            struct keylist : PAIRS... {};
            
            template<typename KEY , typename VALUE> static tml::function<VALUE> test( tml::maps::pair<KEY,VALUE>* ); 



            template<typename K> using at = typename decltype(map::test<K>(static_cast<keylist*>(nullptr)))::type; //Searches for matching overload on bases (PAIRS)
            template<typename K> using contains = tml::boolean<!std::is_same<at<K>,map_base::not_found>::value>;
        };
        
        template<typename M , typename T> using at = typename M::template at<T>;
        template<typename M , typename T> using contains = typename M::template contains<T>;  
    }
}

#endif	/* MAP_HPP */

