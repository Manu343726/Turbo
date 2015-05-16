#include "core_concepts.hpp"
#include "new_eval.hpp"
#include "to_string.hpp"

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

static_assert(!tml::concepts::value<void>(), "???");
static_assert(tml::concepts::value<std::true_type>(), "???");
static_assert(!tml::concepts::metafunction<std::true_type>(), "???");
static_assert(tml::concepts::metafunction_class<nullary_void>(), "???");
static_assert(tml::concepts::metafunction_class<pack_length>(), "???");
static_assert(!tml::concepts::atomic<pack_length>(), "???");
static_assert(!tml::concepts::atomic<nullary_void>(), "???");
static_assert(!tml::concepts::atomic<nullary_void::apply>(), "???");

static_assert(!tml::concepts::value<int>(), "???");
static_assert(!tml::concepts::aggregate<int>(), "???");
static_assert(!tml::concepts::metafunction<int>(), "???");
static_assert(!tml::concepts::metafunction_class<int>(), "???");

static_assert(tml::concepts::atomic<int>(), "???");
static_assert(!tml::concepts::atomic<std::false_type>(), "???");


using i = tml::eval<nullary_void>;

int main()
{
    std::cout << tml::to_string<i>() << std::endl;
}


