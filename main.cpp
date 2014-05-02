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
#include "algorithm.hpp"

#include <type_traits>
#include <typeinfo>
#include <iostream>

using namespace tml::placeholders;

template<typename... ARGS>
struct f
{
    using result = tml::list<ARGS...>;
};

template<typename T>
struct size_of : public tml::function<tml::size_tt<sizeof(T)>>
{};

//tml::let ussage example:
struct X {}; //Variable
struct Y {}; //Variable
struct Z {}; //Variable

using lambda = tml::lambda<_1,f<_1,f<_1,_1,char>,_1>>;
using lambda_result = tml::eval<lambda,bool>;


template<typename T , typename U>
struct equal : public tml::function<typename std::is_same<T,U>::type>
{};


using i = tml::Int<0>;
using j = Int<0>;

using r1 = tml::map<tml::lambda<_1,size_of<_1>>,tml::list<char,bool,int>>;
using r2 = tml::map<tml::lambda<_1,tml::make_list<_1,size_of<_1>>>,tml::list<char,bool,int>>;
using r3 = tml::filter<tml::lambda<_1,equal<_1,int>>,tml::list<char,float,int>>;

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

    
    /*
     * A transform function is just a function which takes one element and returns a
     * transformation of that:
     */
    
    template<typename T>
    struct add_pointer : public tml::function<T*>
    {};
    
    template<typename T>
    struct add_const_lvalue_ref : public tml::function<const T&>
    {};
    
    template<typename T>
    struct add_const : public tml::function<const T>
    {};
    
    template<typename T , typename...>
    struct identity : public tml::function<T>
    {
       static_assert( !std::is_same<size_of<double>,T>::value , "Called!" );
    };
    
    template<typename T>
    struct square : public tml::function<T>
    {};
    
    using example_call_1 = tml::transform<add_pointer<_1>   ,tml::list<float,int,double>>;
    using example_call_2 = tml::transform<tml::lazy<add_const_lvalue_ref>,tml::list<float,int,double>>;
    using example_call_3 = tml::transform<tml::lambda<_1,f<tml::function<_1>,tml::function<add_pointer<_1>>,char>> ,tml::list<char,float,int,double>>;
    
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
    
    
    
    template<typename V>
    using increment = tml::function<std::integral_constant<int,V::value + 1>>;
    
    using result = tml::map<tml::lazy<increment> , tml::list<std::integral_constant<int,0>,
                                                             std::integral_constant<int,1>,
                                                             std::integral_constant<int,2>,
                                                             std::integral_constant<int,3>
                                                            >
                           >;
    
    
    using s = tml::foldl<tml::multi_lambda<_1,_2,
                                           
int main()
{
    std::cout << tml::impl::demangle( typeid( r1 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( r2 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( r3 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( let ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( multi_let ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( multi_let_result<int,int,int> ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( lambda_result ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( result ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_1 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_2 ).name() ) << std::endl;
    std::cout << tml::impl::demangle( typeid( example_call_3 ).name() ) << std::endl;
}

