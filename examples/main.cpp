
#include "core.hpp"
#include "tick/builder.h"
#include <iostream>


TICK_TRAIT(has_apply_template)
{
    template<class T>
    auto require(const T& x) -> valid<
        has_template<T::template apply>
    >;
};

TICK_TRAIT(has_apply_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        typename T::apply
    >;
};


TICK_TRAIT(is_metafunction_class)
{
    template<typename T>
    auto require(const T& x) -> valid<
        is_true<std::integral_constant<bool,has_apply_type<T>::value || has_apply_template<T>::value>>
    >;
};

struct nullary_void
{
    struct apply
    {
        using type = void;
    };
};

struct pack_length
{
    template<typename... Ts>
    struct apply
    {
        using type = std::integral_constant<std::size_t, sizeof...(Ts)>;
    };
};

static_assert(concepts::is_metafunction_class<nullary_void>(), "???");
static_assert(concepts::is_metafunction_class<pack_length>(), "???");

template<concepts::is_metafunction_class F, typename... Args>
struct call
{
    using type = $(F, Args...);
};

int main()
{
    std::cout << tml::to_string<typename call<void>::type>() << std::endl;
    std::cout << tml::to_string<typename call<pack_length, int, int, int>::type>() << std::endl;
}


