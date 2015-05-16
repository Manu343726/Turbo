//
// Created by manu343726 on 16/05/15.
//

#ifndef TURBO_NEW_EVAL_HPP
#define TURBO_NEW_EVAL_HPP

#include "core_concepts.hpp"

namespace tml {
    namespace {
        template<Auto... Args>
        struct eval_;
    }

    template<Auto... Args>
    using eval = typename eval_<Args...>::type;

    namespace {
        template<Auto T>
        struct expand_
        {
            using type = T;
        };

        template<Template T, Auto... Args>
        struct expand_<T<Args...>>
        {
            using type = T<typename eval_<Args>::type...>;
        };

        template<Auto T>
        using expand = typename expand_<T>::type;

        template<tml::concepts::value V>
        struct eval_<V>
        {
            using type = V;
        };

        template<tml::concepts::atomic A>
        struct eval_<A>
        {
            using type = A;
        };

        template<tml::concepts::metafunction M>
        struct eval_<M>
        {
            using type = typename expand<M>::type;
        };

        template<tml::concepts::nullary_metafunction_class C>
        struct eval_<C>
        {
            using type = typename C::apply::type;
        };

        template<tml::concepts::non_nullary_metafunction_class C, Auto... Args>
        struct eval_<C, Args...>
        {
            using type = typename C::template apply<Args...>::type;
        };
    }
}


#endif //TURBO_NEW_EVAL_HPP
