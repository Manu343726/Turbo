
#include "core.hpp"
#include "haskell/data/list.hpp"

#include <iostream>

using namespace tml::haskell;

template<typename Value, typename Type>
constexpr bool Matches()
{
    return is_a<Type, Value>::value;
}

template<Matches<tml::haskell::List> list>
struct get_list
{
    using type = $(list);
};

using l = typename get_list<$(List::Variadic, $i(1))>::type;

int main()
{
    std::cout << tml::to_string<l>() << std::endl;
}


