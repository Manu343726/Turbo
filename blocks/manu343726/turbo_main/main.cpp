
#include "manu343726/turbo_core/turbo_core.hpp"
#include "manu343726/turbo_computing/float.hpp"

//using operator""_TURBO_FLOAT_LITERAL;

#include <tick/requires.h>

#include <iostream>

template<typename metatype, typename ctor, typename value = ctor>
struct boxed_type
{
	using Type = metatype;
	using Ctor = ctor;
	using Value = value;

	struct noexpand {}; //Prevent evaluation during expression parameter expansion

	struct apply
	{
		using type = Value;
	};
};

#define $i(x) std::integral_constant<decltype(x),x>
#define $f(x) FLOAT(x)

template<typename Type, typename BoxedValue>
struct is_a : std::false_type {};

template<typename Type, typename Ctor, typename Value>
struct is_a<Type, boxed_type<Type,Ctor,Value>> : std::true_type {};

template<typename Ctor, typename BoxedValue>
struct constructed_by : std::false_type {};

template<typename Type, typename Ctor, typename Value>
struct constructed_by<Ctor, boxed_type<Type,Ctor,Value>> : std::true_type {};

struct Maybe
{
	struct Just
	{
		template<typename T>
		struct apply
		{
			using type = boxed_type<Maybe, Just, T>;
		};
	};

	struct Nothing
	{
		using type = boxed_type<Maybe, Nothing>;
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
		using type = A;
	};
};

using a = $(f, int);
using b = $(a, char);
using c = $(b, bool);

struct List
{
	struct Cat
	{
		template<typename Head, typename Tail, typename=void>
		struct apply;

		template<typename Head, typename Tail>
		struct apply<Head, Tail,
				     IS_A(List,Tail)
				    >
		{
			using type = boxed_type<List, List::Cat, std::pair<Head,$(Tail)>>;
		};
	};

	struct Nil
	{
		using type = boxed_type<List, Nil>;
	};

	struct Variadic
	{
		template<typename Head, typename... Tail>
		struct apply
		{
			using type = $(List::Cat, Head, $(Variadic, Tail...));
		};

		template<typename Head>
		struct apply<Head>
		{
			using type = $(List::Cat, Head, $(List::Nil));
		};
	};
};

struct Boolean
{
	struct True
	{
		using type = boxed_type<Boolean, True, std::true_type>;
	};

	struct False
	{
		using type = boxed_type<Boolean, False, std::false_type>;
	};
};

struct Number
{
	struct Int
	{
		template<typename N>
		struct apply;

		template<typename T, T V>
		struct apply<std::integral_constant<T,V>>
		{
			using type = boxed_type<Number, Int, std::integral_constant<T,V>>;
		};

		template<tml::floating::sign_t Sign, tml::floating::exponent_t Exp, tml::floating::mantissa_t Mantissa>
		struct apply<tml::floating::number<Sign, Exp, Mantissa>>
		{
			static constexpr int sign = static_cast<bool>(Sign) ? 1 : -1;

			using type = boxed_type<Number, Int,
					                std::integral_constant<int, static_cast<int>(sign * (Mantissa >> Exp))>>;
		};
	};

	struct Float
	{
		template<typename N>
		struct apply;

		template<typename T, T V>
		struct apply<std::integral_constant<T,V>>
		{
			using type = boxed_type<Number, Int, tml::floating::integer<V>>;
		};

		template<tml::floating::sign_t Sign, tml::floating::exponent_t Exp, tml::floating::mantissa_t Mantissa>
		struct apply<tml::floating::number<Sign, Exp, Mantissa>>
		{
			using type = boxed_type<Number, Int, tml::floating::number<Sign, Exp, Mantissa>>;
		};
	};
};

template<std::int64_t NUMBER , std::uint64_t DECIMALS>
struct decimal
{
	using number = typename tml::floating::integer<NUMBER>;

	using decimals_i = tml::integral_constant<std::uint64_t,DECIMALS>;
	using decimals_f = tml::floating::integer<DECIMALS>;
	using digits     = $(tml::add<$(tml::log10<decimals_i>),$(tml::one<decimals_i>)>);
	using divisor    = tml::eval<tml::pow<tml::floating::integer<10>,digits>>;

	using decimals_value = tml::eval<tml::div<decimals_f,divisor>>;

	using result = tml::eval<tml::add<number,decimals_value>>;
};

using list = $(List::Variadic, $i(1), $i(2), $i(3), $i(4));
//using a = $(Number::Float, $f(3.141592));

using x = $(decimal<3,1>);

int main()
{
	std::cout << tml::to_string<list>() << std::endl;
	std::cout << tml::to_string<$(tml::log10<$(tml::add<$i(1),$i(2)>)>)>() << std::endl;
}


