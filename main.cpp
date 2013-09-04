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
#include "numeric_iterators.hpp"

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

using input = mpl::list<mpl::integer<0>,mpl::integer<1>,mpl::integer<2>,mpl::integer<3>,mpl::integer<4>,mpl::integer<5>,mpl::integer<6>,mpl::integer<7>>;
using begin = mpl::begin<input>;
using end   = mpl::end<input>;

template<typename CURRENT_VALUE , typename PREVIOUS_RESULT>
struct fibonacci_kernel
{
    using fib_n_1 = typename PREVIOUS_RESULT::first;
    using fib_n_2 = typename PREVIOUS_RESULT::second;
    
    using fib_n = decltype( fib_n_1() + fib_n_2() );
    
    using result = mpl::pair<fib_n,fib_n_1>;
};

using fib = typename mpl::for_loop<mpl::make_integer_forward_iterator<0> , mpl::make_integer_backward_iterator<2> , mpl::pair<mpl::integer<0>,mpl::integer<0>> , fibonacci_kernel>::result;

template<typename CURRENT_VALUE , typename PREVIOUS_RESULT>
struct simple_kernel
{
    using result = decltype( CURRENT_VALUE() + PREVIOUS_RESULT() );
};

using simple_numeric_for = typename mpl::for_loop<mpl::make_integer_forward_iterator<0> , mpl::make_integer_forward_iterator<20> , mpl::integer<0> , simple_kernel>::result;


using are_equal = mpl::equal<mpl::make_integer_forward_iterator<10>,mpl::next<mpl::make_integer_forward_iterator<9>>>;

int main()
{
    std::cout << mpl::to_string<simple_numeric_for>() << std::endl;
    std::cout << std::boolalpha << are_equal::value << std::endl;
    
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
    
    
    
    std::cout << "begin: " << mpl::to_string<begin>() << std::endl;
    std::cout << "begin (next): " << mpl::to_string<mpl::next<begin>>() << "(Value = " << mpl::to_string<mpl::dereference<mpl::next<begin>>>() << ")" << std::endl;
    std::cout << "end: " << mpl::to_string<end>() << std::endl;
    
    std::cout << "for_loop text input: " << mpl::to_string<mpl::next<mpl::next<mpl::make_integer_forward_iterator<0>>>>() << std::endl;
    //std::cout << "for_loop text result: " << mpl::to_string<fib>() << std::endl;
}

