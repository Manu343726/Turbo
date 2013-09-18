/* 
 * File:   transformations.hpp
 * Author: manu343726
 *
 * Created on 18 de septiembre de 2013, 15:12
 */

#ifndef TRANSFORMATIONS_HPP
#define	TRANSFORMATIONS_HPP

#include "operators.hpp"


namespace math
{
    struct x_axis {};
    struct y_axis {};
    struct z_axis {};
    
    
    template<typename TRANSLATTION>
    struct translate_t;
    
    template<typename SCALE>
    struct scale_t;
    
    template<typename ANGLE , typename AXIS = mpl::no_type>
    struct rotate_t;
    
    template<typename TRANSLATION>
    using translate = typename translate_t<TRANSLATION>::result;

    template<typename SCALE>
    using scale = typename scale_t<SCALE>::result;

    template<typename ANGLE>
    using rotate = typename rotate_t<ANGLE>::result;
}

#endif	/* TRANSFORMATIONS_HPP */

