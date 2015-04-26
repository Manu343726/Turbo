//
// Created by manu343726 on 18/03/15.
//

#ifndef _TURBO_MAYBE_HPP_
#define _TURBO_MAYBE_HPP_

#include <manu343726/turbo_core/eval.hpp>
#include <manu343726/turbo_core/haskell/typeclasses/monad.hpp>

namespace tml
{
    namespace haskell
    {
        template<typename T>
        struct Just;

        struct Nothing;

        struct Maybe
        {
            struct Return
            {
                template<typename U>
                struct apply
                {
                    using type = Just<U>;
                };
            };

            struct Bind
            {
                template<typename M, typename G>
                struct apply;

                template<typename G>
                struct apply<Nothing, G>
                {
                    using type = Nothing;
                };

                template<typename M, typename G>
                struct apply<Just<M>, G>
                {
                    using type = tml::eval<G, M>;
                };
            };
        };

        template<typename T>
        struct Just : public Maybe
        {};

        struct Nothing : public Maybe
        {};
    }
}

#endif //_TURBO_MAYBE_HPP_
