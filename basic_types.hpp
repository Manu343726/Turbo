/* 
 * File:   basic_traits.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 14:08
 */

#ifndef BASIC_TRAITS_HPP
#define	BASIC_TRAITS_HPP

#include "value_t.hpp"

#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <sstream>

namespace make_type_macro
{
    #define MAKE_TYPE(name , type) template<type VALUE> \
                                   using name = mpl::value_t<type,VALUE>; \
                                   using name##_type = type
}

namespace mpl
{
    struct no_type{};
    
    template<typename T>
    struct type_t
    {
        using type = T;
    };
    
    
    template<typename T , typename U>
    struct pair
    {
        using first  = T;
        using second = U;
    };
    
    MAKE_TYPE(ucharacter         , unsigned char);
    MAKE_TYPE(character          , char);
    MAKE_TYPE(uinteger           , unsigned int);
    MAKE_TYPE(integer            , int);
    MAKE_TYPE(ulong_integer      , unsigned long int);
    MAKE_TYPE(long_integer       , long int);
    MAKE_TYPE(ulong_long_integer , unsigned long long int);
    MAKE_TYPE(long_long_integer  , long long int);
    MAKE_TYPE(boolean            , bool);
    MAKE_TYPE(size_t             , std::size_t);
  
    
    using false_type = mpl::boolean<false>;
    using true_type  = mpl::boolean<true>;
}

#endif	/* BASIC_TRAITS_HPP */

