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

#ifndef ALGORITHM_HPP
#define	ALGORITHM_HPP

#include "core.hpp"
#include "list_core.hpp"


TURBO_DEFINE_FUNCTION( length         , (typename... Ts)                                 , (Ts...)                , 
	                  (tml::size_t<sizeof...(Ts)>) 
	                 );

TURBO_DEFINE_FUNCTION( concat         , (typename LHS  , typename RHS)                   , (LHS , RHS)            );
TURBO_DEFINE_FUNCTION( split          , (typename T    , typename INDEX)                 , (T , INDEX)            );
TURBO_DEFINE_FUNCTION( push_back      , (typename LIST , typename T)                     , (LIST , T)             );
TURBO_DEFINE_FUNCTION( push_front     , (typename LIST , typename T)                     , (LIST , T)             );
TURBO_DEFINE_FUNCTION( insert         , (typename LIST , typename POSITION , typename T) , (LIST , POSITION , T)  );
TURBO_DEFINE_FUNCTION( pop_back       , (typename LIST)                                  , (LIST)                 );
TURBO_DEFINE_FUNCTION( pop_front      , (typename LIST)                                  , (LIST)                 );
TURBO_DEFINE_FUNCTION( erase          , (typename LIST , typename POSITION)              , (LIST , POSITION)      );

TURBO_DEFINE_FUNCTION( sort           , (typename T)                                     , (T)                    );
TURBO_DEFINE_FUNCTION( random_shuffle , (typename T)                                     , (T)                    );
  

TURBO_DEFINE_FUNCTION( find           , (typename BEGIN , typename END , typename VALUE) , (BEGIN , END , VALUE)  ,
	                  (tml::for_each<BEGIN,END,tml::function,tml::bind<tml::equal,VALUE>>) 
                     );  
#endif	/* ALGORITHM_HPP */

