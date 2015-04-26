
#include "manu343726/turbo_core/turbo_core.hpp"
#include "manu343726/turbo_core/haskell/data/list.hpp"

#include <iostream>

using namespace tml::haskell;

using list = $(List::Variadic, $i(1), $i(2), $i(3), $i(4));

int main()
{
	std::cout << tml::to_string<list>() << std::endl;
}


