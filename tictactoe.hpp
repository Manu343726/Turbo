/* 
 * File:   tictactoe.hpp
 * Author: manu343726
 *
 * Created on 18 de octubre de 2013, 22:55
 */

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
        
        using empty = mpl::wrapper<mpl::uinteger<0>,hash>;
        using x     = mpl::wrapper<mpl::uinteger<1>,hash>;
        using o     = mpl::wrapper<mpl::uinteger<2>,hash>;
    }
    
    namespace game_state
    {
        struct hash {};
        
        using not_finished = mpl::wrapper<mpl::uinteger<0>,hash>;
        using no_movements = mpl::wrapper<mpl::uinteger<1>,hash>;
        using x_wins       = mpl::wrapper<mpl::uinteger<2>,hash>;
        using o_wins       = mpl::wrapper<mpl::uinteger<3>,hash>;
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
    : public mpl::function<mpl::conditional<decltype( mpl::equal<CELL_00,cell::empty>() || mpl::equal<CELL_10,cell::empty>() || mpl::equal<CELL_20,cell::empty>() ||
                                                      mpl::equal<CELL_01,cell::empty>() || mpl::equal<CELL_11,cell::empty>() || mpl::equal<CELL_21,cell::empty>() ||
                                                      mpl::equal<CELL_02,cell::empty>() || mpl::equal<CELL_12,cell::empty>() || mpl::equal<CELL_22,cell::empty>() ) ,
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
    : public mpl::function<game_state::o_wins>
    {};
    
    //First row specialization (For x):
    template<typename CELL_01 , typename CELL_11 , typename CELL_21 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<cell::x , cell::x , cell::x ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public mpl::function<game_state::x_wins>
    {};
    
    
    
    //Second row specialization (For o):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    cell::o , cell::o , cell::o ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public mpl::function<game_state::o_wins>
    {};
    
    //Second row specialization (For x):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_02 , typename CELL_12 , typename CELL_22>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    cell::x , cell::x , cell::x ,
                                    CELL_02 , CELL_12 , CELL_22>>
    : public mpl::function<game_state::x_wins>
    {};
    
    
    
    //Third row specialization (For o):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_01 , typename CELL_11 , typename CELL_21>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    cell::o , cell::o , cell::o >>
    : public mpl::function<game_state::o_wins>
    {};
    
    //Third row specialization (For x):
    template<typename CELL_00 , typename CELL_10 , typename CELL_20 , 
             typename CELL_01 , typename CELL_11 , typename CELL_21>
    struct check_game_state_t<board<CELL_00 , CELL_10 , CELL_20 ,
                                    CELL_01 , CELL_11 , CELL_21 ,
                                    cell::x , cell::x , cell::x >>
    : public mpl::function<game_state::x_wins>
    {};
    
    template<typename BOARD>
    using check_game_state = typename check_game_state_t<BOARD>::result;
    
    
    
    
    
    namespace 
    {

        
        template<typename BOARD , typename TURN>
        struct compute_next_boards_t
        {
            template<typename T>
            using cell_filter = mpl::equal<T , tictactoe::cell::empty>;
            
            template<typename CURRENT_CELL , typename EMPTY_CELLS_LIST>
            struct search_empty_cells_kernel : public mpl::no_abort_kernel
            {
                using result = mpl::conditional<mpl::equal<CURRENT_CELL,tictactoe::cell::empty> ,
                        mpl::a
            };
        };
    }
}

namespace mpl
{
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::game_state::no_movements>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "no_movements";
        }
    };
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::game_state::not_finished>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "not_finished";
        }
    };
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::game_state::o_wins>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "o_wins";
        }
    };
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::game_state::x_wins>::value>,tictactoe::game_state::hash>>
    {
        operator std::string()
        {
            return "x_wins";
        }
    };
    
    
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::cell::empty>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return " ";
        }
    };
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::cell::o>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return "o";
        }
    };
    
    template<>
    struct to_string_t<mpl::wrapper_t<mpl::uinteger<mpl::result_of<tictactoe::cell::x>::value>,tictactoe::cell::hash>>
    {
        operator std::string()
        {
            return "x";
        }
    };
}

#endif	/* TICTACTOE_HPP */

