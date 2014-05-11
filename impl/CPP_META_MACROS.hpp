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

/*
 * This file defines a set of utility macros to do some metaprogramming with the C preprocessor.
 */

#ifndef CPP_META_MACROS_HPP
#define	CPP_META_MACROS_HPP

/*
 * Removes the enclosing parens of a macro parameter
 * 
 * This library defines some utilities as C macros. That macros should work with C++ templates, which
 * don't fit well in macro calls due to the commas used to separate template argumments. 
 * For example:
 * 
 *     #define identity(x) x //A simple one-argumment macro
 *  
 *     using i = identity( std::integral_constant<int,0> ); //Preprocessor error here
 * 
 * The identity() macro has one parameter only, but when we pass it a std::integral_constant instance, 
 * the preprocessor rejects it saying the macro expects one parameter, not two. The comma which separate
 * the template argumments is preprocessed as the comma of macro argumments separation.
 * 
 * To solve this problem, one could enclose the template using parenthesis, like this:
 * 
 *     identity( (std::integral_constant<int,0>) )
 * 
 * But now the macro argumment includes a pair of parens, and the macro is expanded in that way, so
 * (following with the example above) the expansion of that identity() usage is:
 *     
 *     (std::integral_constant<int,0>)
 * 
 * Which isn't what we expect (Even if in this example is not a problem. In other complex cases the parens could
 * rely on some evaluating problems). So to evaluate such template macro argumments, we need to remove the parens
 * of the macro argumment, in other words, "strip" the argumment.
 * 
 * The proper implementation of a macro which expects template argumments should use this STRIP_PARENS() macro to
 * evaluate the template argumments. Of course any macro which expects a template parameter should strip that parameter,
 * and the user should enclose the parameter with parens:
 * 
 *     #define identity(x) STRIP_PARENS(x) //Proper implementation of the identity() macro example.
 * 
 *     using i = identity( (std::integral_constant<int,0>) );
 */
#define STRIP_PARENS(...) __VA_ARGS__




/*
 * The following macros are utilities to select a given argumment from a set of macro argumments.
 * Up to 8 overloads are defined, using the following syntax:
 * 
 *     SELECT_ARG_N()
 * 
 * Where N is the index of the argumment on the argumment set (Starting at 1).
 * If the selected index is out of bounds, the behaviour is undefined (Preprocessing may fail).
 * 
 * This macro expands to the Nth argumment, for example:
 * 
 *     SELECT_ARG_2( a , b , c , d )
 * 
 * expands to b.
 */

#define SELECT_ARG_1( _1 , ... ) _1
#define SELECT_ARG_2( _1 , _2 , ... ) _2
#define SELECT_ARG_3( _1 , _2 , _3 ... ) _3
#define SELECT_ARG_4( _1 , _2 , _3 , _4 , ... ) _4
#define SELECT_ARG_5( _1 , _2 , _3 , _4 , _5 , ... ) _5
#define SELECT_ARG_6( _1 , _2 , _3 , _4 , _5 , _6 , ... ) _6
#define SELECT_ARG_7( _1 , _2 , _3 , _4 , _5 , _6 , _7 , ... ) _7
#define SELECT_ARG_8( _1 , _2 , _3 , _4 , _5 , _6 , _7 , _8 , ... ) _8



/*
 * This simple macro concatenates two tokens:
 */
#define CAT_TOKENS_IMPL(x,y) x ## y
#define CAT_TOKENS(x,y) CAT_TOKENS_IMPL(x,y)


/*
 * This simple macro expands a macro parameter
 */
#define EXPAND_MACRO(x) x


/*
 * This macro generates an unique identifier based on the line which the macro is expanded:
 */
#define UNIQUE_IDENTIFIER_LINE( id ) CAT_TOKENS( id , __LINE__ )

/*
 * This macro generates an unique identifier based on the internal preprocessor counter:
 */
#define UNIQUE_IDENTIFIER_COUNTER( id ) CAT_TOKENS( id , __COUNTER__ )



#endif	/* CPP_META_MACROS_HPP */

