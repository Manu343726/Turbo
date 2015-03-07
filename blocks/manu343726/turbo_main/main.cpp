
#include "manu343726/turbo_core/turbo_core.hpp"

#include <iostream>

using namespace tml::placeholders;

template<typename... Args>
struct mappable_list
{
	template<typename... Fs>
	struct apply
	{
		using result = mappable_list<tml::eval<Fs(Args)>...>;
	};

	template<typename F>
	struct apply<F>
	{
		using result = mappable_list<tml::eval<F(Args)>...>;
	};
};

struct list_category
{
	template<typename L>
	struct compute;

	template<template<typename...> class L, typename... Args>
	struct compute<L<Args...>>
	{
		using result = tml::lazy<L>;
	};

	template<typename List>
	struct apply
	{
		using result = typename compute<List>::result;
	};
};

struct list_cat
{
	template<typename A, typename B>
	struct apply
	{
		using result = tml::list<A,B>;
	};

	template<template<typename...> class A, typename... AArgs,
	         template<typename...> class B, typename... BArgs
	        >
	struct apply<A<AArgs...>, B<BArgs...>>
	{
		using result = tml::list<AArgs...,BArgs...>;
	};

	template<template<typename...> class L, typename... AArgs,
	                                        typename... BArgs
	        >
	struct apply<L<AArgs...>, L<BArgs...>>
	{
		using result = L<AArgs...,BArgs...>;
	};
};

struct identity
{
	template<typename T>
	struct apply
	{
		using result = T;
	};
};

struct ftor
{
	template<typename From, typename To>
	struct apply
	{
		using result = To;
	};

	template<typename... FromArgs, typename... ToArgs,
			 template<typename...> class From, template<typename...> class To
	        >
	struct apply<From<FromArgs...>, To<ToArgs...>>
	{
		using result = To<FromArgs...>;
	};
};

template<typename... Args>
struct cat_fn
{
	using result = tml::eval<list_cat(Args...)>;
};

template<typename... Ts>
struct foldable_list
{
	template<typename F, typename E>
	struct apply
	{
		template<typename S, typename... Args>
		struct foldl;

		template<typename S, typename Head, typename... Tail>
		struct foldl<S,Head,Tail...>
		{
			using result = typename foldl<tml::eval<F(S,Head)>,Tail...>::result;
		};

		template<typename S>
		struct foldl<S>
		{
			using result = S;
		};

		using result = typename foldl<E,Ts...>::result;
	};
};

template<typename... Fs>
struct Continuation
{
	struct X{};
	struct Y{};

	template<typename Start>
	struct apply
	{
		using list = foldable_list<Fs...>;
		using result = tml::eval<list(tml::lambda<X,Y, Y(X)>,Start)>;
	};
};

template<typename Start, typename... Fs>
using Do = tml::eval<Continuation<Fs...>(Start)>;

template<typename T, typename List>
using push_front = Do<List,                             //Start with List
                      list_category,                    //Strip List out (Return its "clothes", i.e. template<typename...> class L)
                      tml::lambda<_1, _1(T)>,           //Wrap T with List clothes (i.e. return L<T>)
                      tml::lambda<_1,list_cat(_1,List)> //Concat L<T> and List
                     >;


struct mul_2
{
	template<typename N>
	struct apply
	{
		using result = tml::eval<tml::mul<N,tml::Int<2>>>;
	};
};

template<typename T>
struct no_decay
{

};

template<typename T>
struct array_of
{
	template<typename N>
	struct apply
	{
		using result = no_decay<T[N::value]>;
	};
};

template<typename T>
struct Return
{
	template<typename... Args>
	struct apply
	{
		using result = tml::eval<T>;
	};
};

int main()
{
	using numbers = tml::integer_range<2,10>;

	using numbers2 = Do<push_front<tml::Int<1>, numbers>,
						tml::lambda<_1, ftor(_1, mappable_list<_>)>,
						tml::lambda<_1, _1(mul_2)>,
						tml::lambda<_1, _1(Return<ftor(_1,tml::list<_>)>)>,
						tml::lambda<_1, ftor(_1,tml::list<_>)>
					   >;

	std::cout << tml::to_string<numbers>() << std::endl;
	std::cout << tml::to_string<numbers2>() << std::endl;
}