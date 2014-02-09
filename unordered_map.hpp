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

#ifndef UNORDERED_MAP_HPP
#define	UNORDERED_MAP_HPP

#include "core.hpp"
#include "list.hpp"

namespace tml
{
    template<typename... PAIRS>
    class unordered_map;
    
    template<typename... KEYS , typename... VALUES>
    class unordered_map<tml::pair<KEYS,VALUES>...>
    {
    private:
        using keys   = tml::list<KEYS...>;
        using values = tml::list<VALUES...>;
        
    public:
        template<typename KEY>
        using get_value = tml::type_at<values,tml::index_of<keys,KEY>::value>;
        
        template<typename KEY>
        using get_pair  = tml::pair<KEY,get_value<KEY>>;
    };
}

#endif	/* UNORDERED_MAP_HPP */

