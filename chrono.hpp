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

#ifndef TIME_HPP
#define	TIME_HPP

#include "core.hpp"

namespace tml
{
    namespace chrono
    {
        
        template<unsigned int NUM , unsigned int DENOM = 1>
        struct ratio
        {
            using numerator   = NUM;
            using denominator = DENOM;
        };
        
        template<typename RATIO>
        using num = typename RATIO::numerator;
        
        template<typename RATIO>
        using denom = typename RATIO::denominator;
        
        
        template<unsigned int LHS_N , unsigned int LHS_D , unsigned int RHS_N , unsigned int RHS_D>
        struct add_t<tml::chrono::ratio<LHS_N,LHS_D>,
                     tml::chrono::ratio<RHS_N,RHS_D>>
                : public tml::function<tml::chrono::ratio<LHS_N*RHS_D + RHS_N*LHS_D , LHS_D*RHS_D>> {};
                
        template<unsigned int LHS_N , unsigned int LHS_D , unsigned int RHS_N , unsigned int RHS_D>
        struct sub_t<tml::chrono::ratio<LHS_N,LHS_D>,
                     tml::chrono::ratio<RHS_N,RHS_D>>
                : public tml::function<tml::chrono::ratio<LHS_N*RHS_D + RHS_N*LHS_D , LHS_D*RHS_D>> {};
                
        template<unsigned int LHS_N , unsigned int LHS_D , unsigned int RHS_N , unsigned int RHS_D>
        struct mul_t<tml::chrono::ratio<LHS_N,LHS_D>,
                     tml::chrono::ratio<RHS_N,RHS_D>>
                : public tml::function<tml::chrono::ratio<LHS_N*RHS_N , LHS_D*RHS_D>> {};
                
        template<unsigned int LHS_N , unsigned int LHS_D , unsigned int RHS_N , unsigned int RHS_D>
        struct div_t<tml::chrono::ratio<LHS_N,LHS_D>,
                     tml::chrono::ratio<RHS_N,RHS_D>>
                : public tml::function<tml::chrono::ratio<LHS_N*RHS_D , LHS_D*RHS_N>> {};
        
        
        template<typename TICKS , typename RATIO>
        struct duration
        {
            using ticks = TICKS;
            using ratio = RATIO;
        };
        
                
        TURBO_DEFINE_FUNCTION( duration_cast , 
                               ( typename DURATION , typename DEST_RATIO = tml::chrono::ratio<1> )
                               ( DURATION,DEST_RATIO ) ,
                               ( tml::chrono::duration<typename DURATION::ticks,decltype( typename DURATION::ratio() / DEST_RATIO() )> )
                             );
        
        
        
        
        
        using nanoseconds  = tml::chrono::ratio<1,1000000000>;
        using microseconds = tml::chrono::ratio<1,1000000>;
        using miliseconds  = tml::chrono::ratio<1,1000>;
        using seconds      = tml::chrono::ratio<1>;
        using minutes      = tml::chrono::ratio<60>;
        using hours        = tml::chrono::ratio<3600>;
        using days         = tml::chrono::ratio<24*3600>;
        using weeks        = tml::chrono::ratio<7*24*3600>;
        
        
        template<typename LHS_TICKS , typename LHS_RATIO , typename RHS_TICKS , typename RHS_RATIO>
        struct add_t<tml::chrono::duration<LHS_TICKS,LHS_RATIO>,
                     tml::chrono::duration<RHS_TICKS,RHS_RATIO>>
                : public tml::function<tml::duration<
        
        
        
        
        
    }
    
    
    template<typename LHS_TICKS , typename LHS_RATIO , typename RHS_TICKS , typename RHS_RATIO>
    struct add_t<tml::chrono::duration<LHS_TICKS,LHS_RATIO>,
                 tml::chrono::duration<RHS_TICKS,RHS_RATIO>>
                : public tml::function<tml::duration<tml::chrono::duration_cast<
}

#endif	/* TIME_HPP */

