//
// Created by manu343726 on 22/04/15.
//

#ifndef TURBO_LIST_HPP
#define TURBO_LIST_HPP

#include "manu343726/turbo_core/haskell/lang/boxed_type.hpp"

namespace tml
{
    namespace haskell
    {
        struct List
        {
            struct Cat
            {
                template<typename Head, typename Tail, typename=void>
                struct apply;

                template<typename Head, typename Tail>
                struct apply<Head, Tail,
                        TURBO_IS_A(List,Tail)
                >
                {
                    using type = boxed_type<List, List::Cat, std::pair<Head,$(Tail)>>;
                };
            };

            struct Nil
            {
                using type = boxed_type<List, Nil>;
            };

            struct Variadic
            {
                template<typename Head, typename... Tail>
                struct apply
                {
                    using type = $(List::Cat, Head, $(Variadic, Tail...));
                };

                template<typename Head>
                struct apply<Head>
                {
                    using type = $(List::Cat, Head, $(List::Nil));
                };
            };
        };

    }
}

#endif //TURBO_LIST_HPP
