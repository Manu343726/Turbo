/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 13:16
 */

#include "newfile.hpp"
#include "basic_types.hpp"
#include "operators.hpp"
#include "expressions.hpp"
#include "list.hpp"
#include "for.hpp"

#include <iostream>

template<typename T>
void execute(const base<T>& ref)
{
    ref.foo();
}

using a = mpl::integer<0>;
using b = mpl::integer<1>;
using c = mpl::integer<2>;

using d = mpl::add<mpl::add<a,b>,c>;

using e = decltype( (a() + b() + c()) * mpl::integer<2>() );

using f = decltype( mpl::false_type() && mpl::true_type() || !mpl::false_type() && mpl::uinteger<-10>() < mpl::integer<12>());

using list1 = mpl::list<mpl::integer<0>,mpl::list<mpl::boolean<true>,mpl::character<'a'>>,mpl::empty_list,f,e,d>;
using list2 = mpl::list<mpl::integer<0>,mpl::list<mpl::boolean<true>,mpl::character<'a'>>,mpl::empty_list,f,e,d>;
using list3 = mpl::list<char,bool,int,long int,float>;


using input = mpl::list<mpl::integer<0>,mpl::integer<1>,mpl::integer<2>,mpl::integer<3>>;


template<typename CURRENT_VALUE , typename PREVIOUS_RESULT>
struct add_kernel
{
    using result = mpl::add<CURRENT_VALUE,PREVIOUS_RESULT>;
};

using sumatorio = typename mpl::for_loop<mpl::begin<input> , mpl::end<input> , mpl::integer<0> , add_kernel>::result;

int main()
{
    Foo foo;
    Bar bar;
    Quux quux;
    
    execute( foo );
    execute( bar );
    execute( quux );
    
    std::cout << "a: " << a::value << std::endl;
    std::cout << "b: " << b::value << std::endl;
    std::cout << "c: " << c::value << std::endl;
    std::cout << "d: " << d::value << std::endl;
    std::cout << "e: " << e::value << std::endl;
    std::cout << std::boolalpha << "f: " << f::value << std::endl;
    
    std::cout << "legth: " << list1::lenght << " " << mpl::to_string<list1>() << std::endl;
    
    std::cout << "legth: " << list3::lenght << " " << mpl::to_string<list3>() << std::endl;
    std::cout << mpl::to_string<mpl::split_left<list3,mpl::size_t<2>>>() << std::endl;
    std::cout << mpl::to_string<mpl::split_right<list3,mpl::size_t<2>>>() << std::endl;
    
    std::cout << mpl::to_string<f>() << std::endl;
    
    static_assert( mpl::is_value<mpl::integer<0>>::value , "");
}

