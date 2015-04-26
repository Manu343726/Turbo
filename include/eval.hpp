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

#ifndef EVAL_HPP
#define	EVAL_HPP

#include <type_traits>

#include "manu343726/turbo_core/list.hpp"
#include "manu343726/turbo_core/enable_if.hpp"
#include "manu343726/turbo_core/function.hpp"
#include "manu343726/turbo_core/chameleon.hpp"

#include <boost/preprocessor.hpp>

#define TURBO_EVAL_ARGS_APPLY_NO_ARGS(...) >
#define TURBO_EVAL_ARGS_APPLY_ARGS(...) ,__VA_ARGS__>

#define TURBO_EVAL_ARGS_APPLY(...) BOOST_PP_IIF( \
    BOOST_PP_GREATER(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 1), \
        TURBO_EVAL_ARGS_APPLY_ARGS, TURBO_EVAL_ARGS_APPLY_NO_ARGS)

#define TURBO_EVAL_ARGS_APPLY_CALLER(...) TURBO_EVAL_ARGS_APPLY(__,__VA_ARGS__)(__VA_ARGS__)


#define $(...) tml::eval<__VA_ARGS__>

namespace tml
{
    
    /*
     * This metafunction says if one type of expression has overrided the default
     * behaviour of tml::eval. Every user-defined type which wants to override 
     * tml::eval should be registered specializing this metafunction inheriting
     * from tml::true_type.
     */
    template<typename E>
    struct overrides_eval : public tml::false_type
    {};
    
    namespace impl
    {
        /*
         * Here we implement the user-side tml::eval<> metafunction. 
         * 
         * The purpose of that metafunction is to evaluate any kind of expression.
         * That serves to evaluate functions and other functional expressions.
         */   

         template<typename... Args>
         struct args_list {};

         using no_args = args_list<>;

        /*
         * The implementation has three parameters:
         * 
         *  - E: The expression to be evaluated.
         * 
         *  - ARGS: evaluate could be used as a high-order metafunction to evaluate a given
         *             function entity with the specified parameters. This variadic pack is that
         *             set of parameters. The result of the evaluation is the result of evaluating
         *             the functional expresion E with the specified ARGS... arguments.
         *             Note that in this case the argumments are evaluated too (Just for the case they are
         *             functional expressions).
         * 
         *  - SFINAE_FALGS: Parameters used to enable/disable certain specializations.
         * 
         *  Of course this metafunction is a function too, so it stores the result of the evaluation in a 'result' member type.
         */
        template<typename E , typename ARGS , typename SFINAE_FLAGS = tml::sfinae_return>
        struct eval
        {
            using result = E;
        };
        
        /*
         * Non-decayed function pointer types (i.e. R(ARGS...) ) are used as a shorthand for a metafunction call F(ARGS...).
         * So they should be discarded to not match the simple expression tml::eval specialization.
         */
        template<typename T>
        struct is_function_ptr_type : public tml::false_type
        {};
        
        template<typename F , typename... ARGS>
        struct is_function_ptr_type<F(ARGS...)> : public tml::true_type
        {};

        /* This is the most simple case: There are no evaluation parameters (So the expression could be any
         * kind of expression, not just a function) BUT the flag says the expression is not a function.
         * The result of evaluating such expression is the expression itself.
         */
        template<typename E>
        struct eval<E,no_args,
                    TURBO_SFINAE_ALL(
                                     DISABLE_IF(tml::overrides_eval<E>),
                                     DISABLE_IF(tml::is_function<E>),
                                     DISABLE_IF(is_function_ptr_type<E>),
                                     DISABLE_IF(tml::is_aggregate<E>),
                                     DISABLE_IF(tml::is_metafunction_class<E>)
                                    )
                   >
        {
            using result = E;
        };

        template<typename E, typename Args, typename = void>
        struct expand
        {
            using result = E;
        };

        template<typename E, typename Args>
        struct expand<E,Args,TURBO_ENABLE_IF(tml::is_expandible<E>)>
        {
            using result = typename eval<E,Args>::result;
        };

        template<typename E>
        struct eval<E,no_args,
                TURBO_SFINAE_ALL(
                        DISABLE_IF(tml::overrides_eval<E>),
                        ENABLE_IF(tml::is_function<E>),
                        DISABLE_IF(is_function_ptr_type<E>),
                        DISABLE_IF(tml::is_aggregate<E>),
                        DISABLE_IF(tml::is_metafunction_class<E>)
                )
        >
        {
            template<typename T, bool is_stl_function = tml::is_stl_function<T>::value>
            struct call
            {
                using result = typename T::type;
            };

            template<typename T>
            struct call<T, false>
            {
                using result = typename T::result;
            };

            using result = typename call<E>::result;
        };
        

        template<template<typename...> class F , typename... ARGS>
        struct eval<F<ARGS...>,no_args,
                    TURBO_SFINAE_ALL(DISABLE_IF(tml::overrides_eval<F<ARGS...>>),
                                     ENABLE_IF(tml::is_aggregate<F<ARGS...>>),
                                     ENABLE_IF(tml::is_turbo_function<F<ARGS...>>),
                                     DISABLE_IF(tml::is_metafunction_class<F<ARGS...>>)
                                    )
                   > 
        {
            using f = F<typename expand<ARGS,no_args>::result...>;

            template<typename T, bool is_stl_function = tml::is_stl_function<f>::value>
            struct call
            {
                using result = typename T::type;
            };

            template<typename T>
            struct call<T, false>
            {
                using result = typename T::result;
            };

            using result = typename call<f>::result;
        };
        
        /*
         * This specialization matches the case when the expression passed is a parametrized
         * expression (But not a function).
         * 
         * The parameters are evaluated too (Could be functional/parametrized expressions too) to evaluate the entire
         * expression recursively.
         */
        template<template<typename...> class E , typename... ARGS>
        struct eval<E<ARGS...>,no_args,
                    TURBO_SFINAE_ALL(
                                     DISABLE_IF(tml::overrides_eval<E<ARGS...>>),
                                     DISABLE_IF(tml::is_function<E<ARGS...>>),
                                     ENABLE_IF(tml::is_aggregate<E<ARGS...>>),
                                     DISABLE_IF(tml::is_metafunction_class<E<ARGS...>>)
                                    )
                   > : 
                   public tml::function<E<typename expand<ARGS,no_args>::result...>>
        {};

        /*
         * This is the case when the expression passed is a function, and a set of parameters (At least one) is 
         * passed to evaluate the function with. 
         *
         * Note that the parameters of the function call are evaluated too.
         * 
         * The result is the result of evaluating the function with that parameters.
         */
        template<template<typename...> class F , typename... PLACEHOLDERS , typename ARG , typename... ARGS>
        struct eval<F<PLACEHOLDERS...> , args_list<ARG,ARGS...>,
                    TURBO_SFINAE_ALL(
                                     DISABLE_IF(tml::overrides_eval<F<PLACEHOLDERS...>>),
                                     ENABLE_IF(tml::is_function<F<PLACEHOLDERS...>>),
                                     DISABLE_IF(tml::is_metafunction_class<F<PLACEHOLDERS...>>)
                                    )
                   > : 
                   public F<typename expand<ARG,no_args>::result,
                            typename expand<ARGS,no_args>::result...
                           >
        {
            
        };
        
        /*
         * This is the case when the expression passed is a parametrized expression (And not a function),
         * and a set of parameters (At least one) is passed to evaluate the expression with. 
         * 
         * Note that the parameters of the function call are evaluated too.
         */
        template<template<typename...> class E , typename... PLACEHOLDERS , typename ARG , typename... ARGS>
        struct eval<E<PLACEHOLDERS...> , args_list<ARG,ARGS...>,
                    TURBO_SFINAE_ALL(
                                     DISABLE_IF(tml::overrides_eval<E<PLACEHOLDERS...>>),
                                     DISABLE_IF(tml::is_function<E<PLACEHOLDERS...>>),
                                     DISABLE_IF(tml::is_metafunction_class<E<PLACEHOLDERS...>>)
                                    )
                   > : 
                   public tml::function<E<typename expand<ARG,no_args>::result,
                                          typename expand<ARGS,no_args>::result...
                                         >
                                       >
        {
            //static_assert( sizeof...(PLACEHOLDERS) == (1 + sizeof...(ARGS)) , "Wrong number of function call parameters." );  
        };

        template<typename F, typename... ARGS>
        struct eval<F, args_list<ARGS...>,
                    TURBO_SFINAE_ALL(ENABLE_IF(tml::is_metafunction_class<F>))
                   >
        {
            //static_assert(sizeof(F) != sizeof(F), "compiler bug!");

            using apply = tml::impl::get_apply<F, typename expand<ARGS,no_args>::result...>;

            template<typename T, bool is_stl_function = tml::is_stl_function<T>::value>
            struct call
            {
                using result = typename T::type;
            };

            template<typename T>
            struct call<T, false>
            {
                using result = typename T::result;
            };

            using result = typename call<apply>::result;
        };
        
        /*
         * Alternative call-like syntax
         */
        template<typename F , typename... ARGS>
        struct eval<F(ARGS...),no_args,
                    TURBO_SFINAE_ALL(
                                     ENABLE_IF( tml::true_type )
                                    )
                   >:
                   public tml::impl::eval<F,args_list<ARGS...>>
        {};

        template<typename F, typename G, typename... Args>
        struct eval<F(*)(G), args_list<Args...>>
        {
            using result = typename eval<F,args_list<typename eval<G,args_list<Args...>>::result>>::result;
        };
    }
    
    /*
     * Metafunction to evaluate expressions.
     * 
     * The purpose of this metafunction is to evaluate homogeneously any kind of expression.
     * Also, this could be used as a high-order metafunction to reevaluate an specified functional
     * expression with a custom set of parameters.
     * 
     * Note that the parameters of the expression will be ignored during reevaluation. 
     * If your intention is to pass the expression to a high order metafunction,
     * you have to fill that parameters even if they will not be used during evaluation.
     * The set of placeholders defined in "placeholders.hpp" could be used for that purpose.
     * Also the template wrapper 'tml::lazy' defined in "lazy.hpp" could be used to pass a template
     * without parameters and pass that parameters later when the evaluation is done.
     * 
     * The metafunction has the following parameters:
     *  - E: The expression to be evaluated.
     * 
     *  - ARGS...: eval could be used as a high-order metafunction to reevaluate a given
     *             function entity with the specified parameters. This variadic pack is that
     *             set of parameters. The result of the evaluation is the result of evaluating
     *             the functional expresion E with the specified ARGS... arguments.
     *             Note that in this case the argumments are evaluated too (Just for the case they are
     *             functional/parametrized expressions).
     */
    template<typename EXPRESSION , typename... ARGS>
    using eval = typename impl::eval<EXPRESSION , impl::args_list<ARGS...>>::result;
    
    
    
    /*
     * Provides delayed evaluation of an expression until its placeholders are substituted by its values.
     * Is a special case for lambda bodies expressions.
     * 
     * Lambda bodies with evaluating expressions such as 'tml::eval<F,_1>' doesn't work because the placeholder is substituted after the 
     * expression evaluation.
     * 
     * The library provides the template tml::delayed_eval<F,ARGS...> for that purpose: It holds a functional expression reevaluation
     * with parameters that may be placeholders. When doing let on such template, tml::let substitutes the letted placeholders with its
     * values, and the tml::delayed_eval template with tml::eval.
     * 
     * NOTE: See the documentation of the specialization of tml::let in "let_expressions.hpp" for more info.
     */
    template<typename... ARGS>
    struct delayed_eval : public tml::value_chameleon
    {
    };

    template<typename... ARGS>
    struct overrides_eval<tml::delayed_eval<ARGS...>> : public tml::true_type
    {};

    namespace impl
    {
        template<typename E, typename... ARGS>
        struct eval<tml::delayed_eval<E,ARGS...>, no_args> : 
            public tml::function<tml::eval<E,ARGS...>>
        {};
    }
    
    /*
     * Simple shorthand alias:
     */
    template<typename... ARGS>
    using deval = tml::delayed_eval<ARGS...>;
}

#endif	/* EVAL_HPP */

