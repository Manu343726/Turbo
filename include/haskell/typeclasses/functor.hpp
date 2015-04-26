//
// Created by manu343726 on 18/03/15.
//

#ifndef _TURBO_FUNCTOR_HPP_
#define _TURBO_FUNCTOR_HPP_

#include <eval.hpp>

namespace tml
{
    namespace haskell
    {
        template<typename T, typename F>
        using fmap = tml::eval<typename T::fmap, F>;
    }
}

#endif //_TURBO_FUNCTOR_HPP_
