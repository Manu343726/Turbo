
#include "manu343726/turbo_core/turbo_core.hpp"

#include <iostream>

using namespace tml::placeholders;

template<typename... Args>
struct List
{
	template<typename... Fs>
	struct apply
	{
		using result = List<tml::eval<Fs(Args)>...>;
	};

	template<typename F>
	struct apply<F>
	{
		using result = tml::eval<F(Args...)>;
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
	struct apply;

	template<typename... FromArgs, typename... ToArgs,
			 template<typename...> class From, template<typename...> class To
	        >
	struct apply<From<FromArgs...>, To<ToArgs...>>
	{
		using result = To<FromArgs...>;
	};
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
struct pipe
{
	template<typename Start>
	struct apply
	{
		using list = foldable_list<Fs...>;
		using result = tml::eval<list(tml::lambda<_1,_2, tml::deval<_2,_1>>,Start)>;
	};
};

template<typename Start

int main()
{
	using fs = tml::repeat<tml::lazy<std::add_pointer>, tml::size_t<10>>; //tml::list of 10 std::add_pointer

	using p = tml::eval<ftor(fs,pipe<_>)>; //Translate tml::list to pipe
	using result = tml::eval<p(int)>; //Run the pipe starting with int

	std::cout << tml::to_string<result>() << std::endl;
}