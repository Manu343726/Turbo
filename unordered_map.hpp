/* 
 * File:   unordered_map.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 14:15
 */

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

