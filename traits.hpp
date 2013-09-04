/* 
 * File:   common.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:21
 */

#ifndef TRAITS_HPP
#define	TRAITS_HPP

#include "basic_types.hpp"

#include <type_traits> //std::common_type

namespace
{
#define MAKE_TRAIT(type_name) template<typename T>                          \
                         struct is_##type_name : public mpl::false_type {}; \
                                                                            \
                         template<mpl::type_name##_type VALUE>              \
                         struct is_##type_name<mpl::type_name<VALUE>> : public mpl::true_type {}
}

namespace mpl
{   
    /*
    MAKE_TRAIT( ubyte );
    MAKE_TRAIT( byte );
    MAKE_TRAIT( uinteger );
    MAKE_TRAIT( integer );
    MAKE_TRAIT( ulong_integer );
    MAKE_TRAIT( long_integer );
    MAKE_TRAIT( ulong_long_integer );
    MAKE_TRAIT( long_long_integer );
    MAKE_TRAIT( boolean );
    MAKE_TRAIT( size_t );
    */
}

#endif	/* TRAITS_HPP */

