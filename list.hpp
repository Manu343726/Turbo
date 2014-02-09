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

#undef max
#undef min

#include "basic_types.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <sstream>

namespace tml
{
    
    /* mpl::list */
    
    template<typename... Ts>
    struct list
    {
        static const std::size_t lenght = sizeof...(Ts);
        static const std::size_t size = lenght;
        static const bool is_empty = lenght == 0;
    };   
        
    using empty_list = tml::list<>;
    
    namespace 
    {
        template<typename LIST>
        struct _to_string;
        
        //length > 2 (Recursive case)
        template<typename HEAD , typename... TAIL>
        struct _to_string<tml::list<HEAD,TAIL...>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<tml::list<HEAD,TAIL...>>)
            {
                return os << tml::to_string<HEAD>() << ',' << _to_string<tml::list<TAIL...>>();
            }       
        };
        
        //length == 1 (Base case)
        template<typename HEAD>
        struct _to_string<tml::list<HEAD>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<tml::list<HEAD>>)
            {
                return os << tml::to_string<HEAD>();
            }       
        };
        
        //length == 0 (Base case)
        template<>
        struct _to_string<tml::empty_list>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<tml::empty_list>)
            {
                return os;
            }       
        };
    }
    
    template<typename... Ts>
    struct to_string_t<tml::list<Ts...>>
    {
        operator std::string() const
        {
            std::stringstream os;
            
            os << '[' << _to_string<tml::list<Ts...>>() << ']';
            
            return os.str();
        }
    };
    
    namespace 
    {
        /* mpl::index_of */
    
        //Forward-declaration:
        template<typename LIST , typename T>
        struct _index_of;

        //Recursive case:
        template<typename HEAD , typename... TAIL , typename T>
        struct _index_of<tml::list<HEAD,TAIL...>,T>
        {
            static const int tmp = _index_of<tml::list<TAIL...>,T>::value;
            static const int value = tmp < 0 ? -1 : tmp + 1;
        };

        //Base case:
        template<typename... TAIL , typename T>
        struct _index_of<tml::list<T,TAIL...>,T> : public tml::integer<0> {};

        //Base case:
        template<typename T>
        struct _index_of<tml::list<>,T> : public tml::integer<-1> {};
        
        
        /* mpl::contains */
        
        template<typename LIST , typename T>
        struct _contains : public tml::boolean<_index_of<LIST,T>::value >= 0> {};


        /* mpl::type_at */

        //Forward declaration:
        template<typename LIST , std::size_t INDEX>
        struct _type_at;

        //Recursive case:
        template<typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _type_at<tml::list<HEAD,TAIL...> , INDEX> : public tml::type_t< typename _type_at<tml::list<TAIL...>,INDEX - 1>::type> {};
        
        //Base case:
        template<typename HEAD , typename... TAIL>
        struct _type_at<tml::list<HEAD,TAIL...>,0> : public tml::type_t<HEAD> {};
        
        //Base case:
        template<std::size_t INDEX>
        struct _type_at<tml::empty_list,INDEX>
        {
            static_assert( tml::empty_list::lenght == 0 , "Index out of bounds");
        };
        
        
        /* mpl::split */
        
        template<typename LEFT_TYPES , typename RIGHT_TYPES , std::size_t INDEX>
        struct _splitter;
        
        //Recursive case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _splitter<tml::list<LEFT_TYPES...>,tml::list<HEAD,TAIL...>,INDEX>
        {
            using next = _splitter<tml::list<LEFT_TYPES...,HEAD> , tml::list<TAIL...> , INDEX - 1>;
            using right = typename next::right;
            using left  = typename next::left;
        };
        
        //Base case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL>
        struct _splitter<tml::list<LEFT_TYPES...> , tml::list<HEAD,TAIL...> , 0>
        {
            using right = tml::list<HEAD,TAIL...>;
            using left  = tml::list<LEFT_TYPES...>;
        };
        
        /* mpl::concat */
        
        template<typename LIST1 , typename LIST2>
        struct _concat;
        
        template<typename... Ts , typename... Us>
        struct _concat<tml::list<Ts...>,tml::list<Us...>> : public tml::type_t<tml::list<Ts...,Us...>>{};
        
        /* mpl::push_back */
        
        template<typename LIST , typename T>
        struct push_back_t;
        
        template<typename... Ts , typename T>
        struct push_back_t<tml::list<Ts...>,T> : public tml::function<tml::list<Ts...,T>> {};
        
        /* mpl::pop_back */
        
        template<typename LIST>
        struct pop_back_t;
        
        template<typename... Ts , typename BACK>
        struct pop_back_t<tml::list<Ts...,BACK>> : public tml::function<tml::list<Ts...>> {};
        
        
        /* mpl::push_front */
        
        template<typename LIST , typename T>
        struct push_front_t;
        
        template<typename... Ts , typename T>
        struct push_front_t<tml::list<Ts...>,T> : public tml::function<tml::list<T,Ts...>> {};
        
        /* mpl::pop_front */
        
        template<typename LIST>
        struct pop_front_t;
        
        template<typename FRONT , typename... Ts>
        struct pop_front_t<tml::list<FRONT,Ts...>> : public tml::function<tml::list<Ts...>> {};
       
    }
    
    
    template<typename LIST , typename T>
    using index_of = tml::integer<_index_of<LIST,T>::value>;
    
    template<typename LIST , typename T>
    using contains = _contains<LIST,T>;
    
    template<typename LIST , typename INDEX>
    using type_at = typename _type_at<LIST,INDEX::value>::type;
    
    template<typename LIST , typename INDEX>
    using split = _splitter<LIST,tml::empty_list,INDEX::value>;
    
    template<typename LIST , typename INDEX>
    using split_left = typename _splitter<tml::empty_list,LIST,INDEX::value>::left;
    
    template<typename LIST , typename INDEX>
    using split_right = typename _splitter<tml::empty_list,LIST,INDEX::value>::right;
    
    template<typename LHS , typename RHS>
    using concat = typename _concat<LHS,RHS>::type;
    
    template<typename LIST , typename T>
    using push_back = tml::result_of<push_back_t<LIST,T>>;
    
    template<typename LIST>
    using pop_back = tml::result_of<pop_back_t<LIST>>;
        
    template<typename LIST , typename T>
    using push_front = tml::result_of<push_front_t<LIST,T>>;
    
    template<typename LIST>
    using pop_front = tml::result_of<pop_front_t<LIST>>;
    
    
    using empty_list = list<>;

    using invalid_list = tml::no_type;
    using invalid_list_item = tml::no_type;



    template<typename HEAD , typename... TAIL>
    struct begin_t<tml::list<HEAD,TAIL...>> : public tml::make_forward_iterator<HEAD,list<TAIL...>> {};

    template<typename... Ts>
    struct end_t<tml::list<Ts...>> : public tml::make_forward_iterator<tml::invalid_list_item , tml::invalid_list> {};

    template<typename... R_TAIL , typename R_HEAD>
    struct rbegin_t<tml::list<R_TAIL...,R_HEAD>> : public tml::make_backward_iterator<tml::list<R_TAIL...> , R_HEAD> {};

    template<typename... Ts>
    struct rend_t<tml::list<Ts...>> : public tml::make_backward_iterator<tml::invalid_list , tml::invalid_list_item> {};
    

    template<typename ITEM , typename HEAD , typename... TAIL>
    struct next_t<tml::forward_iterator<ITEM,tml::list<HEAD,TAIL...>>> : public tml::make_forward_iterator<HEAD,tml::list<TAIL...>> {};

    template<typename ITEM>
    struct next_t<tml::forward_iterator<ITEM,tml::empty_list>> : public tml::make_forward_iterator<tml::invalid_list_item , tml::invalid_list> {};

    template<typename R_HEAD , typename... R_TAIL , typename ITEM>
    struct previous_t<tml::backward_iterator<tml::list<R_TAIL...,R_HEAD>,ITEM>> : public tml::make_backward_iterator<tml::list<R_TAIL...>,R_HEAD> {};

    template<typename ITEM>
    struct previous_t<tml::backward_iterator<tml::empty_list,ITEM>> : public tml::make_backward_iterator<tml::invalid_list , tml::invalid_list_item> {};
    
    
    
    template<typename LIST>
    struct from_list_t;
    
    template<typename LIST>
    using from_list = tml::result_of<from_list_t<LIST>>;
    
    template<typename T>
    struct to_list_t;
    
    template<typename T>
    using to_list = tml::result_of<to_list_t<T>>;
}

#endif	/* LIST_HPP */

