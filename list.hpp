/* 
 * File:   list.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:17
 */

#ifndef LIST_HPP
#define	LIST_HPP

#undef max
#undef min

#include "basic_types.hpp"
#include "iterator.hpp"
#include "to_string.hpp"

#include <sstream>

namespace mpl
{
    
    /* mpl::list */
    
    template<typename... Ts>
    struct list
    {
        static const std::size_t lenght = sizeof...(Ts);
        static const std::size_t size = lenght;
        static const bool is_empty = lenght == 0;
    };   
        
    using empty_list = mpl::list<>;
    
    namespace 
    {
        template<typename LIST>
        struct _to_string;
        
        //length > 2 (Recursive case)
        template<typename HEAD , typename... TAIL>
        struct _to_string<mpl::list<HEAD,TAIL...>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::list<HEAD,TAIL...>>)
            {
                return os << mpl::to_string<HEAD>() << ',' << _to_string<mpl::list<TAIL...>>();
            }       
        };
        
        //length == 1 (Base case)
        template<typename HEAD>
        struct _to_string<mpl::list<HEAD>>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::list<HEAD>>)
            {
                return os << mpl::to_string<HEAD>();
            }       
        };
        
        //length == 0 (Base case)
        template<>
        struct _to_string<mpl::empty_list>
        {
            friend std::ostream& operator<<(std::ostream& os , _to_string<mpl::empty_list>)
            {
                return os;
            }       
        };
    }
    
    template<typename... Ts>
    struct to_string_t<mpl::list<Ts...>>
    {
        operator std::string() const
        {
            std::stringstream os;
            
            os << '[' << _to_string<mpl::list<Ts...>>() << ']';
            
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
        struct _index_of<mpl::list<HEAD,TAIL...>,T>
        {
            static const int tmp = _index_of<mpl::list<TAIL...>,T>::value;
            static const int value = tmp < 0 ? -1 : tmp + 1;
        };

        //Base case:
        template<typename... TAIL , typename T>
        struct _index_of<mpl::list<T,TAIL...>,T> : public mpl::integer<0> {};

        //Base case:
        template<typename T>
        struct _index_of<mpl::list<>,T> : public mpl::integer<-1> {};
        
        
        /* mpl::contains */
        
        template<typename LIST , typename T>
        struct _contains : public mpl::boolean<_index_of<LIST,T>::value >= 0> {};


        /* mpl::type_at */

        //Forward declaration:
        template<typename LIST , std::size_t INDEX>
        struct _type_at;

        //Recursive case:
        template<typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _type_at<mpl::list<HEAD,TAIL...> , INDEX> : public mpl::type_t< typename _type_at<mpl::list<TAIL...>,INDEX - 1>::type> {};
        
        //Base case:
        template<typename HEAD , typename... TAIL>
        struct _type_at<mpl::list<HEAD,TAIL...>,0> : public mpl::type_t<HEAD> {};
        
        //Base case:
        template<std::size_t INDEX>
        struct _type_at<mpl::empty_list,INDEX>
        {
            static_assert( mpl::empty_list::lenght == 0 , "Index out of bounds");
        };
        
        
        /* mpl::split */
        
        template<typename LEFT_TYPES , typename RIGHT_TYPES , std::size_t INDEX>
        struct _splitter;
        
        //Recursive case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL , std::size_t INDEX>
        struct _splitter<mpl::list<LEFT_TYPES...>,mpl::list<HEAD,TAIL...>,INDEX>
        {
            using next = _splitter<mpl::list<LEFT_TYPES...,HEAD> , mpl::list<TAIL...> , INDEX - 1>;
            using right = typename next::right;
            using left  = typename next::left;
        };
        
        //Base case:
        template<typename... LEFT_TYPES , typename HEAD , typename... TAIL>
        struct _splitter<mpl::list<LEFT_TYPES...> , mpl::list<HEAD,TAIL...> , 0>
        {
            using right = mpl::list<HEAD,TAIL...>;
            using left  = mpl::list<LEFT_TYPES...>;
        };
        
        /* mpl::concat */
        
        template<typename LIST1 , typename LIST2>
        struct _concat;
        
        template<typename... Ts , typename... Us>
        struct _concat<mpl::list<Ts...>,mpl::list<Us...>> : public mpl::type_t<mpl::list<Ts...,Us...>>{};
        
        /* mpl::push_back */
        
        template<typename LIST , typename T>
        struct push_back_t;
        
        template<typename... Ts , typename T>
        struct push_back_t<mpl::list<Ts...>,T> : public mpl::function<mpl::list<Ts...,T>> {};
        
        /* mpl::pop_back */
        
        template<typename LIST>
        struct pop_back_t;
        
        template<typename... Ts , typename BACK>
        struct pop_back_t<mpl::list<Ts...,BACK>> : public mpl::function<mpl::list<Ts...>> {};
        
        
        /* mpl::push_front */
        
        template<typename LIST , typename T>
        struct push_front_t;
        
        template<typename... Ts , typename T>
        struct push_front_t<mpl::list<Ts...>,T> : public mpl::function<mpl::list<T,Ts...>> {};
        
        /* mpl::pop_front */
        
        template<typename LIST>
        struct pop_front_t;
        
        template<typename FRONT , typename... Ts>
        struct pop_front_t<mpl::list<FRONT,Ts...>> : public mpl::function<mpl::list<Ts...>> {};
       
    }
    
    
    template<typename LIST , typename T>
    using index_of = mpl::integer<_index_of<LIST,T>::value>;
    
    template<typename LIST , typename T>
    using contains = _contains<LIST,T>;
    
    template<typename LIST , typename INDEX>
    using type_at = typename _type_at<LIST,INDEX::value>::type;
    
    template<typename LIST , typename INDEX>
    using split = _splitter<LIST,mpl::empty_list,INDEX::value>;
    
    template<typename LIST , typename INDEX>
    using split_left = typename _splitter<mpl::empty_list,LIST,INDEX::value>::left;
    
    template<typename LIST , typename INDEX>
    using split_right = typename _splitter<mpl::empty_list,LIST,INDEX::value>::right;
    
    template<typename LHS , typename RHS>
    using concat = typename _concat<LHS,RHS>::type;
    
    template<typename LIST , typename T>
    using push_back = mpl::result_of<push_back_t<LIST,T>>;
    
    template<typename LIST>
    using pop_back = mpl::result_of<pop_back_t<LIST>>;
        
    template<typename LIST , typename T>
    using push_front = mpl::result_of<push_front_t<LIST,T>>;
    
    template<typename LIST>
    using pop_front = mpl::result_of<pop_front_t<LIST>>;
    
    
    using empty_list = list<>;

    using invalid_list = mpl::no_type;
    using invalid_list_item = mpl::no_type;



    template<typename HEAD , typename... TAIL>
    struct begin_t<mpl::list<HEAD,TAIL...>> : public mpl::make_forward_iterator<HEAD,list<TAIL...>> {};

    template<typename... Ts>
    struct end_t<mpl::list<Ts...>> : public mpl::make_forward_iterator<mpl::invalid_list_item , mpl::invalid_list> {};

    template<typename... R_TAIL , typename R_HEAD>
    struct rbegin_t<mpl::list<R_TAIL...,R_HEAD>> : public mpl::make_backward_iterator<mpl::list<R_TAIL...> , R_HEAD> {};

    template<typename... Ts>
    struct rend_t<mpl::list<Ts...>> : public mpl::make_backward_iterator<mpl::invalid_list , mpl::invalid_list_item> {};
    

    template<typename ITEM , typename HEAD , typename... TAIL>
    struct next_t<mpl::forward_iterator<ITEM,mpl::list<HEAD,TAIL...>>> : public mpl::make_forward_iterator<HEAD,mpl::list<TAIL...>> {};

    template<typename ITEM>
    struct next_t<mpl::forward_iterator<ITEM,mpl::empty_list>> : public mpl::make_forward_iterator<mpl::invalid_list_item , mpl::invalid_list> {};

    template<typename R_HEAD , typename... R_TAIL , typename ITEM>
    struct previous_t<mpl::backward_iterator<mpl::list<R_TAIL...,R_HEAD>,ITEM>> : public mpl::make_backward_iterator<mpl::list<R_TAIL...>,R_HEAD> {};

    template<typename ITEM>
    struct previous_t<mpl::backward_iterator<mpl::empty_list,ITEM>> : public mpl::make_backward_iterator<mpl::invalid_list , mpl::invalid_list_item> {};
    
    
    
    template<typename LIST>
    struct from_list_t;
    
    template<typename LIST>
    using from_list = mpl::result_of<from_list_t<LIST>>;
    
    template<typename T>
    struct to_list_t;
    
    template<typename T>
    using to_list = mpl::result_of<to_list_t<T>>;
}

#endif	/* LIST_HPP */

