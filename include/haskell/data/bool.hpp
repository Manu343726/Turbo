//
// Created by manu343726 on 22/04/15.
//

#ifndef TURBO_BOOL_HPP
#define TURBO_BOOL_HPP

#include "manu343726/turbo_core/haskell/lang/boxed_type.hpp"

namespace tml
{
    namespace haskell
    {
        struct Bool
        {
            struct True
            {
                using type = boxed_type<Boolean, True, std::true_type>;
            };

            struct False
            {
                using type = boxed_type<Boolean, False, std::false_type>;
            };
        };
    }
}

#endif //TURBO_BOOL_HPP
