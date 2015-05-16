//
// Created by manu343726 on 16/05/15.
//

#ifndef TURBO_CORE_CONCEPTS_HPP
#define TURBO_CORE_CONCEPTS_HPP

#include <type_traits>
#include <tick/traits.h>

namespace tml
{
    namespace
    {
        template<typename...>
        struct and_;

        template<>
        struct and_<> : std::true_type
        {
        };

        template<typename A>
        struct and_<A> : A
        {
        };

        template<typename Head, typename... Tail>
        struct and_<Head, Tail...> : std::integral_constant<bool, Head::value && and_<Tail...>::value>
        {
        };

        template<typename...>
        struct or_;

        template<>
        struct or_<> : std::false_type
        {
        };

        template<typename A>
        struct or_<A> : A
        {
        };

        template<typename Head, typename... Tail>
        struct or_<Head, Tail...> : std::integral_constant<bool, Head::value || or_<Tail...>::value>
        {
        };

        template<typename A>
        using not_ = std::integral_constant<bool, !A::value>;

        template<typename... Cs>
        using any_ = tick::ops::is_true<or_<Cs...>>;

        template<typename... Cs>
        using all_ = tick::ops::is_true<and_<Cs...>>;

        template<typename... Cs>
        using none_ = tick::ops::is_false<or_<Cs...>>;

        namespace detail
        {
            template<typename... Ts>
            struct any : std::enable_if<(sizeof...(Ts) > 0)>, std::integral_constant<std::size_t, sizeof...(Ts)>
            {
                static_assert(sizeof...(Ts) > 0, "???????");
            };
        }
    }

    template<typename T>
    constexpr bool Auto()
    {
        return true;
    }

    template<template<Auto...> class T>
    constexpr bool Template()
    {
        return true;
    }

    TICK_TRAIT(aggregate)
    {
        template<typename T>
        struct impl : std::false_type
        {
        };

        template<typename... Args, template<typename...> class T>
        struct impl<T<Args...>> : std::true_type
        {
        };

        template<class T>
        auto require(const T &x) -> valid <
        is_true<impl<T>>
        >;
    };

    TICK_TRAIT(value)
    {
        template<typename T, typename = void>
        struct has_value : std::false_type { };

        template<typename T>
        struct has_value<T, decltype(T::value, void())> : std::true_type { };

        template<class T>
        auto require(const T &x) -> valid <
            is_true<has_value<T>>
        >;
    };

    //POSSIBLE BUG IN GCC-CLITE 0.3: is_false<value<T>> does not work, it reads ::value from value<T> as a type instead of a value
    //during is_false instantiation (The "not T::value" expression, check tick/builder.h).
    //Also the simpler valid<decltype(T::value)> implementation did not work too, hence the has_value/has_no_value metafunctions.
    TICK_TRAIT(not_value)
    {
        template<typename T, typename = void>
        struct has_no_value : std::true_type { };

        template<typename T>
        struct has_no_value<T, decltype(T::value, void())> : std::false_type { };

        template<class T>
        auto require(const T &x) -> valid <
            is_true<has_no_value<T>>
        >;
    };

    TICK_TRAIT(metafunction)
    {
        template<class T>
        auto require(const T& x) -> valid<
            is_true<not_value<T>>, //std::integral_constant has a ::type member aliasing to itself.
            tick::ops::has_type<typename T::type>
        >;
    };

    TICK_TRAIT(nullary_metafunction_class)
    {
        template<class T>
        auto require(const T& x) -> valid<
                typename T::apply
        >;
    };

    TICK_TRAIT(non_nullary_metafunction_class)
    {
        template<class T>
        auto require(const T& x) -> valid<
            has_template<T::template apply>
        >;
    };

    TICK_TRAIT(metafunction_class)
    {
        template<class T>
        auto require(const T& x) -> valid<
            any_<nullary_metafunction_class<T>,
                 non_nullary_metafunction_class<T>
                >
        >;
    };

    TICK_TRAIT(atomic)
    {
        template<class T>
        auto require(const T& x) -> valid<
            is_false<nullary_metafunction_class<T>>,
            is_false<aggregate<T>>,
            is_false<metafunction<T>>,
            is_false<metafunction_class<T>>,
            is_true<not_value<T>>
        >;
    };
}

#endif //TURBO_CORE_CONCEPTS_HPP
