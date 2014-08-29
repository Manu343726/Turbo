
#include "manu343726/turbo_core/turbo_core.hpp"

#include <iostream>

using tml::placeholders::_1;
using tml::placeholders::_2;

struct X : public tml::not_evaluated_function_chameleon{}; struct Y : public tml::not_evaluated_function_chameleon{};

using l = tml::lambda<X,Y , tml::add<X,Y>>;

using r = tml::eval<l,tml::Int<1>,tml::Int<2>>;

int main()
{
	std::cout << tml::to_runtime<r>();	
}