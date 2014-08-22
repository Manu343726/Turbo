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

#ifndef LIST_HPP
#define	LIST_HPP

#include "manu343726/turbo_core/integral.hpp"
#include "manu343726/turbo_core/function.hpp"
#include "manu343726/turbo_core/to_string.hpp"
#include "manu343726/turbo_core/iterator.hpp"

#include <cstddef>
#include <string>
#include <sstream>
#include <algorithm>
#include <tuple>

namespace tml
{
    /*
     * Represents a sequence of types.
     * 
     * This header only defines the tml::list container, to be usable without more dependencies.
     * List algorithms and functions are defined inside "algorithm.hpp" and "list_algorithms.hpp"
     */
    template<typename... Ts>
    struct list
    {
        static constexpr const std::size_t length = sizeof...(Ts);
    };
    
    /*
     * Convenient alias for empty lists.
     */
    using empty_list = tml::list<>;
    
    
    /*
     * Creates a list filled with the specified argumments.
     * 
     * This function just returns the list filled with the parameters of the function.
     * Could be usefull if we need to create lists in evaluable contexts (Like an expression that
     * would be evaluated). A tml::list is not a functional expression, so its elements will not
     * be evaluated recursively if the list belong to a functional expression. In fact, the evaluation
     * of a tml::list fails becuase matches the functional expression case, but is not a function
     * (Has no 'result' member function).
     */
    template<typename... Ts>
    struct make_list : public tml::function<tml::list<Ts...>>
    {};
    
    /*
     * tml::to_string override for lists
     */
    namespace impl
    {
        template<typename... Ts>
        struct to_string<tml::list<Ts...>>
        {
            template<typename... Us>
            struct foo
            {};
            
            friend std::ostream& operator<<( std::ostream& os , foo<> )
            {
                return os;
            }
            
            template<typename HEAD , typename... TAIL>
            friend std::ostream& operator<<( std::ostream& os  , foo<HEAD,TAIL...> )
            {
                if( sizeof...(TAIL) > 0 )
                    return os << tml::to_string<HEAD>() << "," << foo<TAIL...>{};
                else
                    return os << tml::to_string<HEAD>();
            }
            
            
            operator std::string() const
            {
                std::stringstream result;
                
                result << "[";
                
                result << foo<Ts...>{};
                
                result << "]";
                
                return result.str();
            }  
        };
        
        /*
         * tml::to_runtime() override for heterogeneous lists
         */
        template<typename... Ts>
        struct runtime_representation<tml::list<Ts...>> : 
            public tml::function<std::tuple<tml::runtime_representation<Ts>...>>
        {};

        template<typename... Ts>
        struct to_runtime<tml::list<Ts...>>
        {
            static const tml::runtime_representation<tml::list<Ts...>>& execute()
            {
                static const auto& tuple = std::make_tuple( tml::to_runtime<Ts>()... );
                
                return tuple;
            }
        };
        
        /*
         * Forward iterator type for lists
         */
        template<typename... Ts>
        struct list_forward_iterator
        {};
    }
    
    
    /*
     * Operations for list iterators
     */
    namespace iterator
    {
        namespace impl
        {
            /*
             * Forward iterators work storing the sequence following the current
             * element (The current element included). Also a 'sequence_end_element'
             * is inserted at the end of this sequence to represent the past than end
             * element.
             * 
             * Note that begin and end of an empty list point to the past than end element
             */
            
            template<typename HEAD , typename... TAIL>
            struct deref<tml::impl::list_forward_iterator<HEAD,TAIL...>> : public tml::function<HEAD>
            {};
            
            template<typename... Ts>
            struct begin<tml::list<Ts...>> :
                public tml::function<tml::impl::list_forward_iterator<Ts...,tml::iterator::sequence_end_element>>
            {};
            
            template<typename... Ts>
            struct end<tml::list<Ts...>> :
                public tml::function<tml::impl::list_forward_iterator<tml::iterator::sequence_end_element>>
            {};
            
            template<typename HEAD , typename... TAIL>
            struct next<tml::impl::list_forward_iterator<HEAD,TAIL...>> :
                public tml::function<tml::impl::list_forward_iterator<TAIL...>>
            {};
            
            /*
             * The next iterator of the end iterator is the end iterator itself:
             */
            template<>
            struct next<tml::impl::list_forward_iterator<tml::iterator::sequence_end_element>> :
                public tml::function<tml::impl::list_forward_iterator<tml::iterator::sequence_end_element>>
            {};
        }
    }
}

#endif	/* LIST_HPP */

