/* 
 * File:   enable_if.hpp
 * Author: manu343726
 *
 * Created on 8 de mayo de 2014, 14:44
 */

#ifndef ENABLE_IF_HPP
#define	ENABLE_IF_HPP

#include "basic_types.hpp"
#include "list.hpp"

namespace tml
{   
    /*
     * This is the return type of any expression doing SFINAE.
     * By default this type is a void alias, because void is the default return
     * type of std::enable_if.
     */
    using sfinae_return = void;

    /*
     * The purpose of this template is to hold multiple SFINAE entities (enable_ifs)
     * on a single template parameter. 
     * The template behaves as a unique std::enable_if itself, which is enabled only
     * if every enable_if passed as parameter is enabled.
     * 
     * The macro TURBO_SFINAE() makes the use of this sfinae container less verbose.
     */

    namespace impl
    {
        /*
         * This template gets a set of 'std::enable_if' and returns a list with the instantiated 
         * 'std::enable_if::type's
         */
        template<typename... IFS>
        using extract_ifs = tml::list<typename IFS::type...>;

        /*
         * This template behaves like a std::enable_if: Produces a ::type member type if a boolean flag is true.
         */
        template<bool flag>
        struct conditional_type
        {
            using type = tml::sfinae_return;
        };

        template<>
        struct conditional_type<false>
        {};

        /*
         * The implementation 'sfinae_container' template takes a set of 'std::enable_if's, a set of their 
         * ::type instantations, and generates a ::type member alias if all 'std::enable_if's are enabled,
         * counting the number of ::type instantations.
         */
        template<typename IFS , typename EXTRACTED_IFS>
        struct sfinae_container;

        template<typename... IFS , typename... EXTRACTED_IFS>
        struct sfinae_container<tml::list<IFS...>,tml::list<EXTRACTED_IFS...>> 
            : public conditional_type<sizeof...(IFS) == sizeof...(EXTRACTED_IFS)>
        {};
    }
}

/*
 * The following macros define some syntactic sugar for using multiple SFINAE instructions
 * via tml::sfinae_container.
 * 
 * The macro TURBO_SFINAE() instantiates the underlying tml::sfinae_container directly, so the cumbersome typename ::type
 * is not needed to be written by the user. Also, the macro TURBO_FUNCTION_SFINAE() declares a prefix "typename = " very convenient
 * when doing SFINAE on function templates.
 * 
 * Both of them are dessigned to take multiple enable_if conditions. The macros ENABLE_IF() and DISABLE_IF() are shorthands for that enablers.
 * 
 * Finally, the library provides the macros TURBO_ENABLE_IF() and TURBO_DISABLE_IF() (And its function template equivalents)
 * for the cases where multiple conditions are not neccesary.
 */
#define TURBO_SFINAE(...) typename tml::impl::sfinae_container<tml::list<__VA_ARGS__>,tml::impl::extract_ifs<__VA_ARGS__>>::type
#define ENABLE_IF(...)  std::enable_if<__VA_ARGS__::value>
#define DISABLE_IF(...) std::enable_if<!__VA_ARGS__::value>
    
#define TURBO_FUNCTION_SFINAE(...) typename = TURBO_SFINAE(__VA_ARGS__)

#define TURBO_ENABLE_IF(...) typename ENABLE_IF(__VA_ARGS__)::type
#define TURBO_DISABLE_IF(...) typename DISABLE_IF(__VA_ARGS__)::type

#define TURBO_ENABLE_FUNCTION_IF(...) typename = TURBO_ENABLE_IF(__VA_ARGS__)
#define TURBO_DISABLE_FUNCTION_IF(...) typename = TURBO_DISABLE_IF(__VA_ARGS__)

#endif	/* ENABLE_IF_HPP */

