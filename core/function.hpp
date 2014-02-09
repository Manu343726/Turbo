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




/* This core header defines one of the key concepts of the library: The metafunction. 
 * On TMP every template with a public type alias could be considered a metafunction. This header defines a unary metafunction template to provide a
 * common interface for the library and the user. The library considers a metafunction any template with a "result" public type alias.
 * 
 * As the TMP functionality provided by the Turbo Library is created defining or specializing metafunctions, a template like tb::function is useful
 * for the user (And the library developer) to make that specializations. Consider a metafunction like this:
 * 
 * template<typename T>
 * struct unsigned_type;
 * 
 * unsigned_type could be a metafunction which gets a basic type and returns its unsigned equivalent. As unsigned_type is a metafunction, the user should 
 * write the result of the function as a "result" type alias for each specialization:
 *
 * template<>
 * struct unsigned_type<char>
 * {
 *     using result = unsigned char;
 * };
 * 
 * That repetitive code could be simplified using the unary metafunction tb::function:
 * 
 * template<>
 * struct unsigned_type<char> : tb::function<unsigned char> {};
 * 
 * Which can be easily readed as: unsigned_type( char ) --> unsigned char.
 * 
 * The Turbo Metaprogramming Library uses a "specialize with function, return with alias" approach to write metafunctions.
 * That approach consits on writting internal (only for implementation) metafunctions, specialize them with the tb::function way described above,
 * and just define a type alias which returns the result of the implementation metafunction:
 * 
 * template<typename T>
 * struct unsigned_type_t;
 * 
 * template<typename T>
 * using unsigned_type = typename unsigned_type_t<T>::type;
 * 
 * template<>
 * struct unsigned_type_t<char> : mpl::function<unsigned char> {};
 * 
 * This approach lets the users of the function to avoid the "typename" keyword required before the "result" template dependent qualified id. 
 * That makes the usage of the metafunctions more confortable and uniform.
 */

#ifndef FUNCTION_HPP
#define	FUNCTION_HPP

namespace tb
{
    template<typename RESULT>
    struct function
    {
        using result = RESULT;
    };
    
    //Type alias to evaluate a function. Just syntactic sugar to avoid typename keyword.
    template<typename FUNCTION>
    using result_of = typename FUNCTION::result;
}


#define remove_parens(...) __VA_ARGS__

/*
 * Utility macro to automatice metafunction definitions
 * 
 * This macro helps two write metafunction definitions. It writes automatically the implementation template and the user interface alias.
 * Given a function name "name", the implementation template is defined as "name_t", and the alias is defined as "name".
 * 
 * The macro is defined as:
 *     define_function( [function_name] , ([function_parameters]) , ([implementation_argumments_bypass]) );
 * 
 * Where:
 *  - function_name: The name of the metafunction. As described above, the implementation metafunction will be declared as "function_name_t".
 *  - function_parameters: Template parameters of the metafunction. The enclosing parenthesis are needed.
 *  - implementation_argumments_bypass: The forward expression which bypasses the alias argumments to the implementation metafunction. This argumment 
 *    controls in which way the parameters are passed (forwarded) to the implementation. This could be interesting in situations where the user function
 *    (the alias) arranges the argumments in one order, but the implementation is easier to write using another order, or if the implementation just ignores
 *    one or more user params and just uses a set of predefined parameters.
 * 
 * 
 * 
 * Here is an example: A metafunction "first_of" which just takes the first argumment and returns it
 * 
 *     define_function( first_of , (typename T , typename U) , (T , U) );
 * 
 * That macro generates the following code:
 * 
 *     template<typename T , typename U>
 *     struct first_of_t;
 * 
 *     template<typename T , typename U>
 *     using first_of = typename first_of_t<T,U>::result;
 * 
 * Now the implementation:
 * 
 *     template<typename T , typename U>
 *     struct first_of_t<T,U> : tb::function<T> {};
 * 
 * And an usage example:
 *     
 *     first_of<char,bool> a; //a is a char
 * 
 */
#define TURBO_DEFINE_FUNCTION( function_name , template_args , function_args ) \
        template< remove_parens template_args >                                \
        struct function_name##_t;                                              \
                                                                               \
        template< remove_parens template_args >                                \
        using function_name = typename function_name##_t< remove_parens function_args >::result

#endif	/* FUNCTION_HPP */

