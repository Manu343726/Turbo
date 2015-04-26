//
// Created by manu343726 on 18/03/15.
//

#ifndef _TURBO_MONAD_HPP_
#define _TURBO_MONAD_HPP_

#include <manu343726/turbo_core/eval.hpp>

namespace tml
{
    namespace haskell
    {
        template<typename A, typename B>
        using Bind = tml::eval<typename A::Bind, A, B>;

        template<typename T>
        using Return = tml::eval<typename T::Return, T>;

        template<typename _Return, typename _Bind>
        struct Monad
        {
            using Return = _Return;
            using Bind = _Bind;
        };
    }
}

#endif //_TURBO_MONAD_HPP_
