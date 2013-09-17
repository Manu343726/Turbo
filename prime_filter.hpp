/* 
 * File:   prime_filter.hpp
 * Author: manu343726
 *
 * Created on 17 de septiembre de 2013, 14:53
 */

#include "basic_types.hpp"
#include "for_loops.hpp"

#ifndef PRIME_FILTER_HPP
#define	PRIME_FILTER_HPP

namespace math
{
    template<typename A , typename B>
    using is_divisible = mpl::boolean<A::value % B::value == 0>;
    
    namespace 
    {
        template<typename T>
        struct _is_prime;

        template<typename T>
        struct _is_prime
        {
            using numbers = mpl::for_each<mpl::make_uinteger_forward_iterator<2> , mpl::make_uinteger_forward_iterator<T::value> , mpl::function>;

            template<typename NUMBERS , typename = mpl::no_type>
            struct __is_prime;

            template<typename HEAD , typename... TAIL>
            struct __is_prime<mpl::list<HEAD,TAIL...> , mpl::no_type> : public mpl::boolean<!is_divisible<T,HEAD>::value && __is_prime<mpl::list<TAIL...>>::value> {}; 

            template<typename foo>
            struct __is_prime<mpl::empty_list,foo> : public mpl::true_type {};

            using result = mpl::boolean<__is_prime<numbers>::value>;
        };

        template<>
        struct _is_prime<mpl::uinteger<0>> : public mpl::function<mpl::false_type> {};

        template<>
        struct _is_prime<mpl::uinteger<1>> : public mpl::function<mpl::false_type> {};

        template<>
        struct _is_prime<mpl::uinteger<2>> : public mpl::function<mpl::true_type> {};
        
        
        /* Specialization for integers */
        
        template<int VALUE>
        struct _is_prime<mpl::integer<VALUE>> : public _is_prime<mpl::uinteger<(unsigned int)VALUE>> {};
    }
    
    template<typename T>
    using is_prime = typename _is_prime<T>::result;
}

#endif	/* PRIME_FILTER_HPP */

