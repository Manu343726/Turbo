//
// Created by manu343726 on 19/03/15.
//

#ifndef _TURBO_FOLDABLE_HPP_
#define _TURBO_FOLDABLE_HPP_

#include <eval.hpp>

namespace tml
{
    namespace haskell
    {
        template<typename F, typename S, typename T>
        using fold = tml::eval<typename T::fold, F, S>;

        template<typename F, typename S, typename T>
        using foldr = tml::eval<typename T::foldr, F, S>;

        template<typename F, typename S, typename T>
        using foldl = tml::eval<typename T::foldl, F, S>;
    }
}

#endif //_TURBO_FOLDABLE_HPP_
