
#include "core.hpp"

template<typename N>
struct fibbo : public tml::function<tml::mul<typename fibbo<tml::sub<N,tml::integer<1>>>::result,N>>
{};

template<>
struct fibbo<tml::integer<1>> : public tml::function<tml::integer<1>>
{};


using result = typename fibbo<tml::integer<5>>::result;

int main(){}
