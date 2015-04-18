
#include "manu343726/turbo_core/turbo_core.hpp"
#include <tick/requires.h>

#include <iostream>

template<typename metatype, typename ctor, typename value = ctor>
struct TypeDecorator
{
	using Type = metatype;
	using Ctor = ctor;
	using Value = value;
};


template<typename Type, typename BoxedValue>
struct is_a : std::false_type {};

template<typename Type, typename Ctor, typename Value>
struct is_a<Type, TypeDecorator<Type,Ctor,Value>> : std::true_type {};

template<typename Ctor, typename BoxedValue>
struct constructed_by : std::false_type {};

template<typename Type, typename Ctor, typename Value>
struct constructed_by<Ctor, TypeDecorator<Type,Ctor,Value>> : std::true_type {};

struct Maybe
{
	struct Just
	{
		template<typename T>
		struct apply
		{
			using type = TypeDecorator<Maybe, Just, T>;
		};
	};

	struct Nothing
	{
		using type = TypeDecorator<Maybe, Nothing>;
	};
};


#define IS_A(...) TICK_CLASS_REQUIRES(is_a<__VA_ARGS__>())
#define MATCHES(...) TICK_CLASS_REQUIRES(constructed_by<__VA_ARGS__>())

struct is_nothing
{
	template<typename T, typename = void>
	struct apply
	{
		typedef struct error {} type;
	};

	template<typename T>
	struct apply<T, MATCHES(Maybe::Just, T)>
	{
		using type = std::false_type;
	};

	template<typename T>
	struct apply<T, MATCHES(Maybe::Nothing, T)>
	{
		using type = std::true_type;
	};
};

using maybe_int = $(Maybe::Just, int);

struct f
{
	template<typename... Head>
	struct apply
	{
		struct type
		{
			template<typename... Tail>
			using apply = f::apply<Head...,Tail...>;
		};
	};

	template<typename A, typename B, typename C>
	struct apply<A, B, C>
	{
		using type = C;
	};
};

using a = $(f, int);
using b = $(a, char);
using c = $(b, bool);

int main()
{
	std::cout << tml::to_string<a>() << std::endl;
	std::cout << tml::to_string<b>() << std::endl;
	std::cout << tml::to_string<c>() << std::endl;
}


