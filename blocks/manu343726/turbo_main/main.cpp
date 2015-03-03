
#include "manu343726/turbo_core/turbo_core.hpp"

#include <iostream>

using namespace tml::placeholders;

template<typename A, typename B, typename C, typename...>
struct third
{
	template<typename _1, typename _2, typename _3, typename...>
	struct apply
	{
		using type = _3;
	};
};

int main()
{
	using l = tml::lambda<_1, _2, _3, _4, tml::deval<_1, _2, _3, _4>>;

	using result = tml::eval<l, tml::lazy<third>, tml::integer<1>, tml::integer<2>, tml::integer<3>>;

	std::cout << tml::to_string<result>() << std::endl;
}