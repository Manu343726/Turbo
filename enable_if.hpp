/* 
 * File:   enable_if.hpp
 * Author: manu343726
 *
 * Created on 8 de mayo de 2014, 14:44
 */

#ifndef ENABLE_IF_HPP
#define	ENABLE_IF_HPP

#define TURBO_ENABLE_IF(...) typename std::enable_if<__VA_ARGS__::value>::type
#define TURBO_DISABLE_IF(...) typename std::enable_if<!__VA_ARGS__::value>::type

#define TURBO_ENABLE_FUNCTION_IF(...) typename = TURBO_ENABLE_IF(__VA_ARGS__)
#define TURBO_DISABLE_FUNCTION_IF(...) typename = TURBO_DISABLE_IF(__VA_ARGS__)

#endif	/* ENABLE_IF_HPP */

