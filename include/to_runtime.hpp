/******************************************************************************
 * Turbo C++11 metaprogramming Library                                         *
 *                                                                             *
 * Copyright (C) 2013 - 2014, Manuel Sánchez Pérez                             *
 *                                                                             *
 * This file is part of The Turbo Library.                                     *
 *                                                                             *
 * The Turbo Library is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation, version 2 of the License.                     *
 *                                                                             *
 * The Turbo Library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with The Turbo Library. If not, see <http://www.gnu.org/licenses/>.   *
 ******************************************************************************/

#ifndef TO_RUNTIME_HPP
#define	TO_RUNTIME_HPP

/*
 * The function tml::to_runtime<T>() returns a value equivalent to
 * a passed compile-time metavalue T. 
 * This function has the purpose of serve as bridge between a compile-time metaprograms
 * and the C++ program expecting the results of that compile-time computation.
 * 
 * tml::to_runtime() calls the static member function execute() of tml::impl::to_runtime<T>.
 * User defined types should specialize that template providing a static member function
 * execute() with no parameters. 
 * 
 * The metafunction tml::runtime_representation specifies the type of the runtime representation
 * of a certain compile-time metavalue.
 */

namespace tml
{
    namespace impl
    {
        template<typename T>
        struct to_runtime
        {
            static_assert( sizeof(T) != sizeof(T) , "You should write a custom to runtime conversion for your type" );
        };
        
        /*
         * Overridable implementation of tml::runtime_representation metafunction. 
         */
        template<typename T>
        struct runtime_representation
        {
            static_assert( sizeof(T) != sizeof(T) , "You should specify the runtime representation of your type" );
        };
    }
    
    /*
     * tml::runtime_representation is provided as a manipulable functor too
     */
    namespace func
    {
        template<typename T>
        using runtime_representation = tml::impl::runtime_representation<T>;
    }
    
    /*
     * This metafunction specifies which is the representation of a certain compile-time meta
     * value at runtime, in other words, the type of the equivalent runtime value.
     * 
     * Note that this specifies the type of the entity which holds the value, not the type of the result of a tml::to_runtime() call.
     * Commonly the runtime values are stored on read-only memory using static variables on tml::impl::to_runtime<T>::execute(), and then that function
     * returns a const reference to them. Thats done to avoid unnecessary copies and ensure inmutability of compile-time computed values.  
     * 
     * The user should override the implementation metafunction tml::impl::runtime_representation
     */
    template<typename T>
    using runtime_representation = typename tml::impl::runtime_representation<T>::type;
    
    /*
     * Returns the runtime equivalent of a compile-time value.
     */
    template<typename T>
    constexpr auto to_runtime() -> decltype( tml::impl::to_runtime<T>::execute() )
    {
        return tml::impl::to_runtime<T>::execute();
    }
}

#endif	/* TO_RUNTIME_HPP */

