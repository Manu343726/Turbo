/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 7 de febrero de 2014, 21:36
 */


/*
#include "core.hpp"

define_function( (typename T , typename U) , common_type , (T , U) );

template<typename T , typename U>
struct common_type_t
{
    using result = T;
};
*/

#define strip(...) __VA_ARGS__
#define define_function(A, B, C) template< strip A, strip C> struct B ## _impl;

define_function( (class X, class Y), foo, (X) )

int main(){}

