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

#include "eval.hpp"
#include "placeholders.hpp"
#include "let_expressions.hpp"
#include "lambda.hpp"

#include "impl/demangle.hpp"
#include "lazy.hpp"

#include "basic_types.hpp"

#include <type_traits>
#include <typeinfo>
#include <iostream>

using namespace tml::placeholders;

template<typename... ARGS>
struct f
{
    using result = tml::impl::list<ARGS...>;
};


//tml::let ussage example:
struct X {}; //Variable
struct Y {}; //Variable
struct Z {}; //Variable

using lambda = tml::lambda<_1,f<_1,f<_1,_1,char>,_1>>;
using lambda_result = tml::eval<lambda,bool>;


using i = tml::Int<0>;
using j = Int<0>;

namespace complex_lambda_example
{
    namespace impl
    {
        template<typename COMPARER , typename ELEMS>
        struct max_impl_high;
        
        template<typename CURRENT_MAX , typename COMPARER , typename ELEMS>
        struct max_impl_low;
        
        
        template<typename COMPARER , typename HEAD , typename... TAIL>
        struct max_impl_high<COMPARER,tml::impl::list<HEAD,TAIL...>> : 
            public max_impl_low<HEAD,COMPARER,tml::impl::list<TAIL...>>
        {};
        
        template<typename CURRENT_MAX , typename COMPARER , typename HEAD , typename... TAIL>
        struct max_impl_low<CURRENT_MAX,COMPARER,tml::impl::list<HEAD,TAIL...>> : 
            public max_impl_low<tml::eval<COMPARER,HEAD,CURRENT_MAX>,
                                COMPARER,tml::impl::list<TAIL...>
                               >
        {};
        
        template<typename CURRENT_MAX , typename COMPARER>
        struct max_impl_low<CURRENT_MAX,COMPARER,tml::impl::list<>> : 
            public tml::function<CURRENT_MAX>
        {};
    }
    
    /*
     * Returns the max elements from a set of elements using the specified criteria
     */
    template<typename COMPARER , typename... ARGS>
    using max = tml::eval<impl::max_impl_high<COMPARER,tml::impl::list<ARGS...>>>;
    
    
    /*
     * A comparer returns the bigger of two parameters based on specific criteria:
     */
   
    template<typename A , typename B>
    using bigger = tml::function<typename std::conditional<(sizeof(A) > sizeof(B)),A,B>::type>;
    
    template<typename A , typename B>
    using less = tml::function<typename std::conditional<(sizeof(A) < sizeof(B)),A,B>::type>;
    
    //using example_call_1 = max<bigger<_1,_2>,int,float,char,double>;
    //using example_call_2 = max<less<_1,_2>,int,float,char,double>;
}

    /* Simple example: TMP equivalent of std::transform */
    template<typename T , typename... ARGS>
    using transform = tml::impl::list<tml::eval<T,ARGS>...>;
    
    /*
     * A transform function is just a function which takes one element and returns a
     * transformation of that:
     */
    
    template<typename T>
    struct add_pointer : public tml::function<T*>
    {};
    
    template<typename T>
    struct add_lvalue_ref : public tml::function<T&>
    {};
    
    template<typename T>
    struct add_const : public tml::function<const T>
    {};
    
    template<typename T>
    struct size_of : public tml::function<std::integral_constant<std::size_t,sizeof(T)>>
    {
    
    };
    
    template<typename T , typename...>
    struct identity : public tml::function<T>
    {
       static_assert( !std::is_same<size_of<double>,T>::value , "Called!" );
    };
    
    template<typename T>
    struct square : public tml::function<T>
    {};
    
    using example_call_1 = transform<add_pointer<_1>   ,float,int,double>;
    using example_call_2 = transform<tml::lazy<add_lvalue_ref>,float,int,double>;
    using example_call_3 = transform<tml::lambda<_1,f<tml::function<_1>,tml::function<add_pointer<_1>>,char>> ,char,float,int,double>;

    static_assert( tml::impl::is_function<add_pointer<_1>>::result , "ERROR" );
    
    using let = tml::let<_1,long long int,square<size_of<_1>>>;
    
    using multi_let = tml::multi_let<_1,_2,_3,
                                     int,int,int,
                                     f<_1,_2,_3>
                                    >;
    
    template<typename... ARGS>
    using multi_let_result = tml::eval<tml::multi_let<_1,_2,_3,
                                                      ARGS...,
                                                      f<_1,_2,_3>
                                                     >
                                      >;
    
    using multi_lambda = tml::multi_lambda<_1,_2,_3 , f<_1,_2,_3>>;
    //using multi_lambda_result = tml::eval<multi_lambda,int,int,int>;
    
    
    template<typename F , typename... ARGS>
    using map = tml::impl::list<tml::eval<F,ARGS>...>;
    
    template<typename V>
    using increment = tml::function<std::integral_constant<int,V::value + 1>>;
    
    using result = map<tml::lazy<increment> , std::integral_constant<int,0>,
                                              std::integral_constant<int,1>,
                                              std::integral_constant<int,2>,
                                              std::integral_constant<int,3>
                      >;
                                           
int main()
{
    
    std::cout << tml::impl::demangle( typeid( let ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( multi_let ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( multi_let_result<int,int,int> ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( lambda_result ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( result ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_1 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_2 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_3 ).name() ) << std::endl;
    
}

