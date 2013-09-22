/* 
 * File:   render.hpp
 * Author: manu343726
 *
 * Created on 19 de septiembre de 2013, 15:23
 */

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
        
        using normal = math::cross_product<mpl::sub<V2,V1> , mpl::sub<V3,V1>>;
    };
    
    template<typename VIEW_VECTOR , typename NORMAL>
    using backface_cull_filter = decltype( mpl::mul<VIEW_VECTOR,NORMAL>() < mpl::zero<typename NORMAL::x>() );
    
    template<typename VIEW_VECTOR , typename TRIANGLE_LIST>
    struct backface_culling_t
    {
        template<typename TRIANGLE>
        using filter = backface_cull_filter<VIEW_VECTOR,typename TRIANGLE::normal>;
        
        using result = mpl::for_each<mpl::begin<TRIANGLE_LIST> , mpl::end<TRIANGLE_LIST> , mpl::function , filter>;
    };
    
    template<typename VIEW_VECTOR , typename TRIANGLE_LIST>
    using backface_culling = typename backface_culling_t<VIEW_VECTOR,TRIANGLE_LIST>::result;
}

#endif	/* RENDER_HPP */

