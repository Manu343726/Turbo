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

#ifndef TYPE_TRAITS_HPP
#define	TYPE_TRAITS_HPP

#include "function.hpp"
#include "basic_types.hpp"
#include "fixed_point.hpp"
#include "lazy.hpp"
#include "utils/assert.hpp"

#include <type_traits>
#include <utility>

namespace tml
{
    namespace impl
    {
        template<typename T>
        struct is_parametrized_expression : public tml::function<tml::false_type>
        {};
        
        template<template<typename...> class E , typename... ARGS>
        struct is_parametrized_expression<E<ARGS...>> : public tml::function<tml::true_type>
        {};
        
        
        template<typename T>
        struct is_integral_constant : public tml::function<tml::false_type>
        {};
        
        template<typename T , T V>
        struct is_integral_constant<tml::integral_constant<T,V>> : public tml::function<tml::true_type>
        {};
        
        template<typename T>
        struct is_fixed_point : public tml::function<tml::false_type>
        {};
        
        template<typename INTEGER_T , INTEGER_T V>
        struct is_fixed_point<tml::fixed_point<INTEGER_T,V>> : public tml::function<tml::true_type>
        {};
        
        
        template<typename T>
        struct is_functor
        {
            template<typename U> static tml::true_type  f( decltype(&U::operator())* );
            template<typename U> static tml::false_type f( ... );
            
            using result = decltype( f<T>( nullptr ) );
        };
        
        
        template<typename T>
        struct is_runtime_function : public tml::impl::is_functor<T>
        {};
        
        template<typename R , typename... ARGS>
        struct is_runtime_function<R(*)(ARGS...)> : public tml::function<tml::true_type>
        {};
        
        template<typename C , typename R , typename... ARGS>
        struct is_runtime_function<R(C::*)(ARGS...)> : public tml::function<tml::true_type>
        {};
        
        
        template<typename R , typename... ARGS>
        struct function_signature_holder
        {
            using return_type = R;
            using args        = tml::list<ARGS...>;
        };
        
        /*
         * This is the default template. Its used to evaluate functors.
         * The other function entities (Function pointers) are treated bellow.
         */
        template<typename F>
        struct function_signature
        {
            TURBO_ASSERT( (tml::eval<tml::impl::is_functor<F>>) , "ERROR: The type specified is not a function" );
            
            template<typename T>
            struct signature_extractor;
            
            template<typename C , typename R , typename... ARGS>
            struct signature_extractor<R(C::*)(ARGS...)> : public tml::impl::function_signature_holder<R,ARGS...>
            {};
            
            template<typename C , typename R , typename... ARGS>
            struct signature_extractor<R(C::*)(ARGS...) const> : public tml::impl::function_signature_holder<R,ARGS...>
            {};
            
            using return_type = typename signature_extractor<decltype(&F::operator())>::return_type;
            using args        = typename signature_extractor<decltype(&F::operator())>::args;
        };
        
        template<typename R , typename... ARGS>
        struct function_signature<R(ARGS...)> : public tml::impl::function_signature_holder<R,ARGS...>
        {};
        
        template<typename R , typename... ARGS>
        struct function_signature<R(*)(ARGS...)> : public tml::impl::function_signature_holder<R,ARGS...>
        {};
        
        template<typename C , typename R , typename... ARGS>
        struct function_signature<R(C::*)(ARGS...)> : public tml::impl::function_signature_holder<R,ARGS...>
        {};
        
        template<typename R , typename... ARGS>
        struct make_function_signature : public tml::function<R(ARGS...)>
        {};
        
        
        template<typename F, typename... ARGS>
        struct is_valid_call 
        {
        private:
            template<typename FF, typename... AA>
            static auto f(int) ->
            decltype( std::declval<FF>()(std::declval<AA>()...), tml::true_type{} );

            template<typename FF, typename... AA>
            static tml::false_type f(...);
        public:
            using result = decltype( f<F,ARGS...>(0) );
        };
    }
    
    namespace func
    {
        /*
         * Checks if a type T is considered a parametrized expression by the Turbo expression
         * evaluation system.
         * 
         * Returns tml::true_type if the type T is a parametrized expression. Returns 
         * tml::false_type otherwise.
         */
        template<typename T>
        using is_parametrized_expression = tml::impl::is_parametrized_expression<T>;
        
        /*
         * Checks if a type T is considered a function by the Turbo expression
         * evaluation system.
         * 
         * Returns tml::true_type if the type T is a metafunction. Returns 
         * tml::false_type otherwise.
         */
        template<typename T>
        using is_metafunction = tml::impl::is_function<T>;
        
        /*
         * Checks if a type T is an integral constant, that is, a basic metavalue
         * (A tml::Int , a tml::Char , etc).
         * 
         * Returns tml::true_type if the type T is an integral constant. Returns 
         * tml::false_type otherwise.
         */
        template<typename T>
        using is_integral_constant = tml::impl::is_integral_constant<T>;
        
        /*
         * Checks if a type T is a fixed-point value.
         * 
         * Returns tml::true_type if the type T is a fixed-point value. Returns 
         * tml::false_type otherwise.
         */
        template<typename T>
        using is_fixed_point = tml::impl::is_fixed_point<T>;
        
        /*
         * Checks if a type T is a function entity (A function pointer, a functor, 
         * a lambda, etc). 
         * Returns tml::true_type if T is a function entity type. Returns 
         * tml::false_type.
         */
        template<typename T>
        using is_runtime_function = tml::impl::is_runtime_function<T>;
        
        /*
         * Given a function entity with type F, returns the type of the argumments of the function entity.
         */
        template<typename F>
        using function_argumments = tml::function<typename tml::impl::function_signature<F>::args>;
        
        /*
         * Given a function entity with type F, returns the return type of the function entity.
         */
        template<typename F>
        using function_return_type = tml::function<typename tml::impl::function_signature<F>::return_type>;
        
        /*
         * Given a return type R and a set of argumments types, returns the corresponding function signature type.
         */
        template<typename R , typename... ARGS>
        using make_function_signature = tml::impl::make_function_signature<R,ARGS...>;
        
        /*
         * Given a function entity type, and a set of argumment types, checks if the call to that
         * function with the parameters is well-formed.
         */
        template<typename F , typename... ARGS>
        using is_valid_call = tml::impl::is_valid_call<F,ARGS...>;
    }
    
    /*
    * Checks if a type T is considered a parametrized expression by the Turbo expression
    * evaluation system.
    * 
    * Returns tml::true_type if the type T is a parametrized expression. Returns 
    * tml::false_type otherwise.
    */
   template<typename T>
   using is_parametrized_expression = tml::eval<tml::func::is_parametrized_expression<T>>;

   /*
    * Checks if a type T is considered a function by the Turbo expression
    * evaluation system.
    * 
    * Returns tml::true_type if the type T is a metafunction. Returns 
    * tml::false_type otherwise.
    */
   template<typename T>
   using is_metafunction = tml::eval<tml::func::is_metafunction<T>>;

   /*
    * Checks if a type T is an integral constant, that is, a basic metavalue
    * (A tml::Int , a tml::Char , etc).
    * 
    * Returns tml::true_type if the type T is an integral constant. Returns 
    * tml::false_type otherwise.
    */
   template<typename T>
   using is_integral_constant = tml::eval<tml::func::is_integral_constant<T>>;
   
   /*
    * Checks if a type T is a fixed-point value.
    * 
    * Returns tml::true_type if the type T is a fixed-point value. Returns 
    * tml::false_type otherwise.
    */
   template<typename T>
   using is_fixed_point = tml::eval<tml::func::is_fixed_point<T>>;

   /*
    * Checks if a type T is a function entity (A function pointer, a functor, 
    * a lambda, etc). 
    * Returns tml::true_type if T is a function entity type. Returns 
    * tml::false_type.
    */
   template<typename T>
   using is_runtime_function = tml::eval<tml::func::is_runtime_function<T>>;

   /*
    * Given a function entity with type F, returns the type of the argumments of the function entity.
    */
   template<typename F>
   using function_argumments = tml::eval<tml::func::function_argumments<F>>;

   /*
    * Given a function entity with type F, returns the return type of the function entity.
    */
   template<typename F>
   using function_return_type = tml::eval<tml::func::function_return_type<F>>;

   /*
    * Given a return type R and a set of argumments types, returns the corresponding function signature type.
    */
   template<typename R , typename... ARGS>
   using make_function_signature = tml::eval<tml::func::make_function_signature<R,ARGS...>>;
   
   /*
    * Given a function entity type, and a set of argumment types, checks if the call to that
    * function with the parameters is well-formed.
    */
   template<typename F , typename... ARGS>
   using is_valid_call = tml::eval<tml::func::is_valid_call<F,ARGS...>>;
}

#endif	/* TYPE_TRAITS_HPP */

