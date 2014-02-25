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
        //Time type tags:
        struct hours   : public tml::uinteger<3600>{}; //Time type ratio
        struct minutes : public tml::uinteger<60>{};
        struct seconds : public tml::uinteger<1>{};
        using  ticks = tml::chrono::seconds; //Time precision is second
        
        
        //Time values:
        
        template<typename COUNT>
        struct hour : public COUNT 
        {
            using time_ratio = tml::chrono::hours;
        };
        
        template<typename COUNT>
        struct minute : public COUNT 
        {
            using time_ratio = tml::chrono::minutes;
        };
        
        template<typename COUNT>
        struct second : public COUNT 
        {
            using time_ratio = tml::chrono::seconds;
        };
        
        template<typename COUNT>
        using tick = tml::chrono::second<COUNT>;
        
      
        TURBO_DEFINE_FUNCTION( time_cast , (typename... ARGS) , (ARGS...) );
        
        template<typename HOUR_VALUE , typename MINUTE_VALUE , typename SECOND_VALUE>
        struct time_cast_t<HOUR_VALUE,MINUTE_VALUE,SECOND_VALUE , tml::chrono::seconds> : public tml::function<tml::chrono::second<decltype( SECOND_VALUE() + tml::uinteger<60>()*MINUTE_VALUE() + tml::uinteger<3600>()*HOUR_VALUE() )>> {};
        
        template<typename HOUR_VALUE , typename MINUTE_VALUE , typename SECOND_VALUE>
        struct time_cast_t<HOUR_VALUE,MINUTE_VALUE,SECOND_VALUE , tml::chrono::minutes> : public tml::function<tml::chrono::minute<decltype( SECOND_VALUE() + tml::uinteger<60>()*MINUTE_VALUE() + tml::uinteger<3600>()*HOUR_VALUE() )>> {};
        
        template<typename HOUR_VALUE , typename MINUTE_VALUE , typename SECOND_VALUE>
        struct time_cast_t<HOUR_VALUE,MINUTE_VALUE,SECOND_VALUE , tml::chrono::hours>   : public tml::function<tml::chrono::hour<HOUR_VALUE::value>> {};
        
        
        template<typename FROM>
        struct time_cast_t<FROM,tml::chrono::seconds> : public tml::function<tml::chrono::second<( FROM::value * typename FROM::time_ratio::value ) / 1>> {};
        
        template<typename FROM>
        struct time_cast_t<FROM,tml::chrono::minutes> : public tml::function<tml::chrono::minute<( FROM::value * typename FROM::time_ratio::value ) / 60>> {};
        
        template<typename FROM>
        struct time_cast_t<FROM,tml::chrono::hours>   : public tml::function<tml::chrono::hour<( FROM::value * typename FROM::time_ratio::value ) / 3600>> {};
        
        
                TURBO_DEFINE_FUNCTION( time_value , (typename... DATA) , (DATA...) );
        
        template<typename HOUR , typename MINUTE , typename SECOND>
        struct time_value_t<HOUR,MINUTE,SECOND>
        {
            using hour_value   = HOUR;
            using minute_value = MINUTE;
            using second_value = SECOND;
            
            using hour_count = HOUR;
            using minute_count  = decltype( minutes() + tml::uinteger<3600>()*hours() );
            using seconds_count = decltype( seconds() + tml::uinteger<60>()*minutes() + tml::uinteger<3600>()*hours() );
        };
        
        template<typename HOUR , typename MINUTE , typename SECOND>
        struct time_point : tml::chrono::time_value<HOUR,MINUTE,SECOND>
        {};
        
        template<typename TICKS>
        struct duration : public tml::chrono::time<TICKS>
        {
        };
        
        
    }
    
    template<typename HOUR_BEGIN , typename MINUTE_BEGIN , typename SECOND_BEGIN ,
                 typename HOUR_END   , typename MINUTE_END   , typename SECOND_END>
    struct sub_t<tml::chrono::time_point<HOUR_END,MINUTE_END,SECOND_END> , 
                 tml::chrono::time_point<HOUR_END,MINUTE_END,SECOND_END>> : public tml::function<tml::chrono::duration<tml::chrono::time_cast<decltype(HOUR_END() - HOUR_BEGIN()),
                                                                                                                                              decltype(HOUR_END() - HOUR_BEGIN()),
                                                                                                                                              decltype(HOUR_END() - HOUR_BEGIN())>>>> {};
}

#endif	/* TIME_HPP */

