//
// Created by manu343726 on 22/04/15.
//

#ifndef TURBO_MAYBE_HPP
#define TURBO_MAYBE_HPP

#include "haskell/lang/boxed_type.hpp"

namespace tml
{
    namespace haskell
    {
        struct Maybe
        {
            struct Just
            {
                template<typename T>
                struct apply
                {
                    using type = boxed_type<Maybe, Just, T>;
                };
            };

            struct Nothing
            {
                using type = boxed_type<Maybe, Nothing>;
            };
        };
    }
}

#endif //TURBO_MAYBE_HPP
