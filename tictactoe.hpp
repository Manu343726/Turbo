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

#ifndef TICTACTOE_HPP
#define	TICTACTOE_HPP

#include "matrix3x3.hpp"
#include "wrapper.hpp"

namespace tictactoe
{
    template<typename... Ts>
    using board = math::matrix3x3<Ts...>;
    
    namespace cell
    {
        struct hash {};
        
        using empty = tml::wrapper<tml::uinteger<0>,hash>;
        using x     = tml::wrapper<tml::uinteger<1>,hash>;
        using o     = tml::wrapper<tml::uinteger<2>,hash>;
    }
    
    namespace game_state
    {
        struct hash {};
        
        using not_finished = tml::wrapper<tml::uinteger<0>,hash>;
        using no_movements = tml::wrapper<tml::uinteger<1>,hash>;
        using x_wins       = tml::wrapper<tml::uinteger<2>,hash>;
        using o_wins       = tml::wrapper<tml::uinteger<3>,hash>;
    }
    
    template<typename BOARD>
    struct check_game_state_t;
    
    //No winner specialization:
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_01 , typename CELL_11 , typename CELL_21 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public tml::function<tml::conditional<decltype( tml::equal<CELL_00,cell::empty>() || tml::equal<CELL_10,cell::empty>() || tml::equal<CELL_20,cell::empty>() ||
                                                      tml::equal<CELL_01,cell::empty>() || tml::equal<CELL_11,cell::empty>() || tml::equal<CELL_21,cell::empty>() ||
                                                      tml::equal<CELL_02,cell::empty>() || tml::equal<CELL_12,cell::empty>() || tml::equal<CELL_22,cell::empty>() ) ,
                                            game_state::not_finished ,
                                            game_state::no_movements>
                          >
    {};
    
    
    
    //First row specialization (For o):
    template<typename CELL_01 , typename CELL_11 , typename CELL_21 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<cell::o , cell::o , cell::o ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public tml::function<game_state::o_wins>
    {};
    
    //First row specialization (For x):
    template<typename CELL_01 , typename CELL_11 , typename CELL_21 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<cell::x , cell::x , cell::x ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public tml::function<game_state::x_wins>
    {};
    
    
    
    //Second row specialization (For o):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    cell::o , cell::o , cell::o ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public tml::function<game_state::o_wins>
    {};
    
    //Second row specialization (For x):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    cell::x , cell::x , cell::x ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public tml::function<game_state::x_wins>
    {};
    
    
    
    //Third row specialization (For o):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_01 , typename CELL_11 , typename CELL_21>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    cell::o , cell::o , cell::o >>
    : public tml::function<game_state::o_wins>
    {};
    
    //Third row specialization (For x):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_01 , typename CELL_11 , typename CELL_21>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    cell::x , cell::x , cell::x >>
    : public tml::function<game_state::x_wins>
    {};
    
    template<typename BOARD>
    using check_game_state = typename check_game_state_t<BOARD>::result;
    
    
    
    
    namespace 
    {

        
        template<typename BOARD , typename TURN>
        struct compute_next_boards_t
        {
            template<typename T>
            using cell_filter = tml::equal<T , tictactoe::cell::empty>;
            
            template<typename CURRENT_CELL , typename EMPTY_CELLS_LIST>
            struct search_empty_cells_kernel : public tml::no_abort_kernel
            {
                using result = tml::conditional<tml::equal<CURRENT_CELL,tictactoe::cell::empty> ,
                                                tml::push_back<EMPTY_CELLS_LIST,TURN> ,
                                                tml::push_back<EMPTY_CELLS_LIST,CURRENT_CELL
                                               >;
            };
        };
    }
}

namespace tml
{
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::game_state::no_movements>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "no_movements";
        }
    };
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::game_state::not_finished>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "not_finished";
        }
    };
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::game_state::o_wins>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "o_wins";
        }
    };
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::game_state::x_wins>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "x_wins";
        }
    };
    
    
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::cell::empty>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return " ";
        }
    };
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::cell::o>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return "o";
        }
    };
    
    template<>
    struct to_string_t<tml::wrapper_t<tml::uinteger<tml::result_of<tictactoe::cell::x>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return "x";
        }
    };
}

#endif	/* TICTACTOE_HPP */

