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

#ifndef FUNCTION_HPP
#define	FUNCTION_HPP

#include <type_traits>
#include <tick/traits.h>

namespace tml
{
    namespace impl
    {
        /*
         * The library assumes that any type with 'result' and/or 'type' member types is a function
         * (A function which result is stored in that 'result'/'type' member).
         * 
         * This is an internal (not designed to be used by the user) type trait to 
         * check the existence of that member type, that is, to check if a type is
         * a function.
         */
        TICK_TRAIT(is_turbo_function)
        {
            template<class T>
            auto require(const T& x) -> valid<
                typename T::type
            >;
        };
        
        /*
         * The library assumes that any type with 'result' and/or 'type' member types is a function
         * (A function which result is stored in that 'result'/'type' member).
         * 
         * This is an internal (not designed to be used by the user) type trait to 
         * check the existence of that member type, that is, to check if a type is
         * a function.
         */
        TICK_TRAIT(is_stl_function)
        {
            template<class T>
            auto require(const T& x) -> valid<
                typename T::type
            >;
        };
        
        /*
         * The library assumes that any type with 'result' and/or 'type' member types is a function
         * (A function which result is stored in that 'result'/'type' member).
         * 
         * This is an internal (not designed to be used by the user) type trait to 
         * check the existence of that member type, that is, to check if a type is
         * a function.
         */
        TICK_TRAIT(is_function)
        {
            template<typename T>
            auto require(const T& x) -> valid<
                is_true<std::integral_constant<bool,tml::impl::is_turbo_function<T>::value || tml::impl::is_stl_function<T>::value>>
            >;
        };
        
        /*
         * Be careful! std::integral_constant also contains a member type 'type' 
         * aliasing the instance itself.
         */
        template<typename T, T V>
        struct is_function<std::integral_constant<T,V>>
        {
            static constexpr bool value = false;
        };

        template<typename T, typename... Args>
        struct get_apply_impl
        {
            using type = typename T::template apply<Args...>;
        };

        template<typename T>
        struct get_apply_impl<T>
        {
            using type = typename T::apply;
        };

        template<typename T, typename... Args>
        using get_apply = typename get_apply_impl<T,Args...>::type;



        TICK_TRAIT(has_apply_template)
        {
            template<class T>
            auto require(const T& x) -> valid<
                has_template<T::template apply>
                >;
        };

        TICK_TRAIT(has_apply_type)
        {
            template<class T>
            auto require(const T& x) -> valid<
                    typename T::apply
            >;
        };


        TICK_TRAIT(is_metafunction_class)
        {
            template<typename T>
            auto require(const T& x) -> valid<
                is_true<std::integral_constant<bool,has_apply_type<T>::value || has_apply_template<T>::value>>
            >;
        };

        TICK_TRAIT(not_expandible)
        {
            template<class T>
            auto require(const T& x) -> valid<
                    typename T::noexpand
            >;
        };
    }
    
    /* User-side tml::is_function type-trait 
     * 
     * Checks if a given expression is a function according to the conventions of the library
     * (Has a 'result'/'type' member type)
     */
    template<typename E>
    using is_function = std::integral_constant<bool,tml::impl::is_function<E>::type>;
    
    /* User-side tml::is_turbo_function type-trait 
     * 
     * Checks if a given expression is a function according to the Turbo conventions
     * (Has a 'result' member type)
     */
    template<typename E>
    using is_turbo_function = std::integral_constant<bool,tml::impl::is_turbo_function<E>::type>;
    
    /* User-side tml::is_turbo_function type-trait 
     * 
     * Checks if a given expression is a function according to the STL conventions
     * (Has a 'type' member type)
     */
    template<typename E>
    using is_stl_function = std::integral_constant<bool,tml::impl::is_stl_function<E>::type>;

    /*
     * Checks if the type T is a metafunction class, that is, has an internal apply metafunction accepting Args... as parameters.
     */
    template<typename T>
    using is_metafunction_class = std::integral_constant<bool, tml::impl::is_metafunction_class<T>::value>;

    /*
     * Checks if the type T can (should) be evaluated during parameter expansion on expression evaluation
     */
    template<typename T>
    using is_expandible = std::integral_constant<bool, !tml::impl::not_expandible<T>::value>;
    
    /*
     * This is a helper metafunction to represent a metafunction in the way the library
     * expects it.
     * 
     * It just takes a value and stores it in a 'result'/'type' member (i.e. acts as an identity function).
     * 
     * Its usefull when declaring user-defined metafunctions and making them working with the rest
     * of the library. Inheriting from this helper is a simple whay to ensure any metafunction
     * has the correct interface.
     */
    template<typename type>
    struct function
    {
        using type = type;
    };
}

#endif	/* FUNCTION_HPP */

