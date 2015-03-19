
#include "manu343726/turbo_core/turbo_core.hpp"
#include "manu343726/turbo_core/haskell/typeclasses/monad.hpp"
#include "manu343726/turbo_core/haskell/typeclasses/maybe.hpp"

#include <iostream>


using namespace tml::haskell;

struct remove_stars
{
	template<typename T>
	struct apply
	{
		using type = Just<T>;
	};

	template<typename T>
	struct apply<T*>
	{
		using type = Just<T>;
	};

	template<typename T, std::size_t N>
	struct apply<T[N]>
	{
		using type = Nothing;
	};
}; /* remove_stars */

struct run
{
	template<typename Current, typename Elem>
	struct apply
	{
		using type = Bind<Current, remove_stars>;
	};
};

using array_t = int[10];

using result = tml::apply_for<run, Just<int*****>, tml::size_t<0>, tml::size_t<10>>;
using error  = tml::apply_for<run, Just<array_t****>, tml::size_t<0>, tml::size_t<10>>;

int main()
{
	std::cout << tml::to_string<result>() << std::endl;
	std::cout << tml::to_string<error>() << std::endl;
}