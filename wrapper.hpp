/* 
 * File:   wrapper.hpp
 * Author: manu343726
 *
 * Created on 20 de octubre de 2013, 18:47
 */

#ifndef WRAPPER_HPP
#define	WRAPPER_HPP

#include "operators.hpp"
#include "to_string.hpp"


namespace mpl
{
    template<typename T , typename HASH = mpl::no_type>
    struct wrapper_t : public mpl::function<T>
    {
        using type = T;
        using hash = HASH;
    };
    
    template<typename T , typename HASH = mpl::no_type>
    using wrapper = wrapper_t<T,HASH>;
    
    template<typename T , typename HASH>
    struct to_string_t<mpl::wrapper<T,HASH>> : public to_string_t<T> {};
}

#endif	/* WRAPPER_HPP */

