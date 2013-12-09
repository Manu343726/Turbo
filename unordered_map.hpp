/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013, Manuel Sánchez Pérez                                    *
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

#include "basic_types.hpp"
#include "list.hpp"

namespace mpl
{
    template<typename... PAIRS>
    class unordered_map;
    
    template<typename... KEYS , typename... VALUES>
    class unordered_map<mpl::pair<KEYS,VALUES>...>
    {
    private:
        using keys   = mpl::list<KEYS...>;
        using values = mpl::list<VALUES...>;
        
    public:
        template<typename KEY>
        using get_value = mpl::type_at<values,mpl::index_of<keys,KEY>::value>;
        
        template<typename KEY>
        using get_pair  = mpl::pair<KEY,get_value<KEY>>;
    };
}

#endif	/* UNORDERED_MAP_HPP */

