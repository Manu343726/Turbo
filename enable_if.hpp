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

namespace tml
{
    /*
     * This is the return type of any expression doing SFINAE.
     * By default this type is a void alias, because void is the default return
     * type of std::enable_if.
     */
    using sfinae_return = void;

    /*
     * The purpose of this list is to hold multiple SFINAE entities (enable_ifs)
     * on a single template parameter.
     */
    template<typename... ARGS>
    using sfinae_list = tml::sfinae_return;
}

#endif	/* ENABLE_IF_HPP */

