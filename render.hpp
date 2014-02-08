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

#ifndef RENDER_HPP
#define	RENDER_HPP

#include "operators.hpp"
#include "vector.hpp"
#include "for_loops.hpp"

namespace gfx
{
    template<typename V1 , typename V2 , typename V3>
    struct triangle
    {
        using v1 = V1;
        using v2 = V2;
        using v3 = V3;
        
        using normal = math::cross_product<tb::sub<V2,V1> , tb::sub<V3,V1>>;
    };
    
    template<typename VIEW_VECTOR , typename NORMAL>
    using backface_cull_filter = decltype( tb::mul<VIEW_VECTOR,NORMAL>() < tb::zero<typename NORMAL::x>() );
    
    template<typename VIEW_VECTOR , typename TRIANGLE_LIST>
    struct backface_culling_t
    {
        template<typename TRIANGLE>
        using filter = backface_cull_filter<VIEW_VECTOR,typename TRIANGLE::normal>;
        
        using result = tb::for_each<tb::begin<TRIANGLE_LIST> , tb::end<TRIANGLE_LIST> , tb::function , filter>;
    };
    
    template<typename VIEW_VECTOR , typename TRIANGLE_LIST>
    using backface_culling = typename backface_culling_t<VIEW_VECTOR,TRIANGLE_LIST>::result;
}

#endif	/* RENDER_HPP */

