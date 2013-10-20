/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

//#define MPL_CUSTOM_FIXED_POINT_DEFAULT_PRECISION 8

#include "to_string.hpp"
#include "tictactoe.hpp"
#include "color.hpp"

#include <iostream>

using x = tictactoe::cell::x;
using o = tictactoe::cell::o;
using e = tictactoe::cell::empty;

using my_board = tictactoe::board<x,e,e,
                                  e,e,e,
                                  o,o,o>;

int main()
{
    std::cout << mpl::to_string<my_board>() << std::endl << std::endl;
    std::cout << mpl::to_string<tictactoe::check_game_state<my_board>>() << std::endl;
}

