
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


int main()
{

	using f = tml::lazy<std::add_pointer>;
	using g = tml::lazy<std::remove_pointer>;

	using result = tml::eval<f(int)>;

	std::cout << tml::to_string<result>() << std::endl;
}