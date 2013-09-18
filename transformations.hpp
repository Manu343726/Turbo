/* 
 * File:   transformations.hpp
 * Author: manu343726
 *
 * Created on 18 de septiembre de 2013, 15:12
 */

#ifndef TRANSFORMATIONS_HPP
#define	TRANSFORMATIONS_HPP

#include "operators.hpp"
#include "trigonometry.hpp"


namespace math
{
    struct x_axis {};
    struct y_axis {};
    struct z_axis {};
    
    struct rotation_2d {};
    
    
    template<typename TRANSLATTION>
    struct translate_t;
    
    template<typename SCALE>
    struct scale_t;
    
    template<typename ANGLE , typename AXIS , typename TRIGONOMETRY_APROX_TERMS>
    struct rotate_t;
    
    template<typename TRANSLATION>
    using translate = typename translate_t<TRANSLATION>::result;

    template<typename SCALE>
    using scale = typename scale_t<SCALE>::result;

    template<typename ANGLE , typename AXIS = math::rotation_2d , typename TRIGONOMETRY_APROX_TERMS = math::default_trigonometry_approximation_terms>
    using rotate = typename rotate_t<mpl::opposite<ANGLE>,AXIS,TRIGONOMETRY_APROX_TERMS>::result; //Apaño: He escrito las matrices de rotación al revés
}

#endif	/* TRANSFORMATIONS_HPP */

