/* 
 * File:   boolean_predicates.hpp
 * Author: manu343726
 *
 * Created on 17 de septiembre de 2013, 1:21
 */

#ifndef BOOLEAN_PREDICATES_HPP
#define	BOOLEAN_PREDICATES_HPP

#include "basic_types.hpp"


namespace mpl
{
    template<typename T>
    using true_predicate = mpl::true_type;
    
    template<typename T>
    using false_predicate = mpl::false_type;
}

#endif	/* BOOLEAN_PREDICATES_HPP */

