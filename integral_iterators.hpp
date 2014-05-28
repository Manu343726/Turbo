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

#ifndef NUMERIC_ITERATORS_HPP
#define	NUMERIC_ITERATORS_HPP

#include "basic_types.hpp"


namespace tml
{
    namespace integral_forward_iterators
    {
        namespace impl
        {
            template<typename T , T V>
            struct iterator
            {};
            
            template<typename V>
            struct iterator_builder;
            
            template<typename T , T V>
            struct iterator_builder<tml::integral_constant<T,V>> :
                public tml::function<tml::integral_forward_iterators::impl::iterator<T,V>>
            {};
        }
        
        template<typename N>
        using make = typename tml::integral_forward_iterators::impl::iterator_builder<N>::result;
        
        template<std::size_t N>
        using make_size_t = tml::integral_forward_iterators::make<tml::size_t<N>>;
        
        template<unsigned char N>
        using make_unsigned_char = tml::integral_forward_iterators::make<tml::UnsignedChar<N>>;
        
        template<char N>
        using make_char = tml::integral_forward_iterators::make<tml::Char<N>>;
        
        template<bool N>
        using make_bool = tml::integral_forward_iterators::make<tml::Bool<N>>;
        
        template<int N>
        using make_int = tml::integral_forward_iterators::make<tml::Int<N>>;
        
        template<int N>
        using make_long_int = tml::integral_forward_iterators::make<tml::LongInt<N>>;
        
        template<int N>
        using make_long_long_int = tml::integral_forward_iterators::make<tml::LongLongInt<N>>;
        
        template<int N>
        using make_unsigned_int = tml::integral_forward_iterators::make<tml::UnsignedInt<N>>;
        
        template<int N>
        using make_unsigned_long_int = tml::integral_forward_iterators::make<tml::UnsignedLongInt<N>>;
        
        template<int N>
        using make_unsigned_long_long_int = tml::integral_forward_iterators::make<tml::UnsignedLongLongInt<N>>;
    }
    
    namespace integral_backward_iterators
    {
        namespace impl
        {
            template<typename T , T V>
            struct iterator
            {};
            
            
            template<typename V>
            struct iterator_builder;
            
            template<typename T , T V>
            struct iterator_builder<tml::integral_constant<T,V>> :
                public tml::function<tml::integral_backward_iterators::impl::iterator<T,V>>
            {};
        }
        
        template<typename N>
        using make = typename tml::integral_backward_iterators::impl::iterator_builder<N>::result;
        
        template<std::size_t N>
        using make_size_t = tml::integral_backward_iterators::make<tml::size_t<N>>;
        
        template<unsigned char N>
        using make_unsigned_char = tml::integral_backward_iterators::make<tml::UnsignedChar<N>>;
        
        template<char N>
        using make_char = tml::integral_backward_iterators::make<tml::Char<N>>;
        
        template<bool N>
        using make_bool = tml::integral_backward_iterators::make<tml::Bool<N>>;
        
        template<int N>
        using make_int = tml::integral_backward_iterators::make<tml::Int<N>>;
        
        template<int N>
        using make_long_int = tml::integral_backward_iterators::make<tml::LongInt<N>>;
        
        template<int N>
        using make_long_long_int = tml::integral_backward_iterators::make<tml::LongLongInt<N>>;
        
        template<int N>
        using make_unsigned_int = tml::integral_backward_iterators::make<tml::UnsignedInt<N>>;
        
        template<int N>
        using make_unsigned_long_int = tml::integral_backward_iterators::make<tml::UnsignedLongInt<N>>;
        
        template<int N>
        using make_unsigned_long_long_int = tml::integral_backward_iterators::make<tml::UnsignedLongLongInt<N>>;
    }
    
    namespace iterator
    {
        namespace impl
        {
            template<typename T , T V>
            struct deref<tml::integral_forward_iterators::impl::iterator<T,V>> : public tml::function<tml::integral_constant<T,V>>
            {};
            
            template<typename T , T V>
            struct deref<tml::integral_backward_iterators::impl::iterator<T,V>> : public tml::function<tml::integral_constant<T,V>>
            {};
            
            template<typename T , T V>
            struct next<tml::integral_forward_iterators::impl::iterator<T,V>> :
                public tml::function<tml::integral_forward_iterators::impl::iterator<T,V+1>>
            {};
            
            template<typename T , T V>
            struct next<tml::integral_backward_iterators::impl::iterator<T,V>> :
                public tml::function<tml::integral_backward_iterators::impl::iterator<T,V-1>>
            {};
        }
    }
}

#endif	/* NUMERIC_ITERATORS_HPP */

