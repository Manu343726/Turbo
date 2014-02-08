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
    using rotate = typename rotate_t<tb::opposite<ANGLE>,AXIS,TRIGONOMETRY_APROX_TERMS>::result; //Apaño: He escrito las matrices de rotación al revés
}

#endif	/* TRANSFORMATIONS_HPP */

