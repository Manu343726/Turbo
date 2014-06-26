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

#include <vector>
#include <typeindex>
#include <unordered_map>
#include <type_traits>
#include <memory>

#include "enable_if.hpp"

namespace tml
{
    namespace runtime
    {
        template<typename BASE>
        struct poly_container
        {   
        public:
            poly_container() = default;
            
            template<typename T , TURBO_ENABLE_FUNCTION_IF( std::is_base_of<BASE,T> )>
            void insert( const T& elem )
            {
                auto& segment = _segments[typeid(T)];
                
                if(!segment) 
                    segment.reset( new polycont_segment<T>{} );
                    
                segment->insert( elem );
            }
            
            template<typename F>
            void for_each( F f ) const
            {
                for( const auto& segment : _segments )
                    segment.second->for_each( f );
            }
            
            template<typename F>
            void mutable_for_each( F f )
            {
                for( auto& segment : _segments )
                    segment.second->mutable_for_each( f );
            }
            
            template<typename F , typename FILTER>
            void for_each_if( F f , FILTER filter ) const
            {
                for_each( [&]( const BASE& elem )
                {
                    if( filter( elem ) ) f( elem );
                });
            }
            
            template<typename F , typename FILTER>
            void mutable_for_each_if( F f , FILTER filter )
            {
                mutable_for_each( [&]( BASE& elem )
                {
                    if( filter( elem ) ) f( elem );
                });
            }
            
        private:
            struct polycont_segment_base
            {
            public:
                template<typename F>
                void for_each( F f ) const
                {
                    const std::size_t elem_size = this->element_size();
                    const std::size_t size      = this->size();

                    for( auto it  = begin() , 
                              end = it + size * elem_size;
                              it != end;
                              it += elem_size )
                    {
                        const auto& elem = *reinterpret_cast<const BASE*>( it );
                        f( elem );
                    }
                }
                
                template<typename F>
                void mutable_for_each( F f )
                {
                    const std::size_t elem_size = this->element_size();
                    const std::size_t size      = this->size();

                    for( auto it  = begin() , 
                              end = it + size * elem_size;
                              it != end;
                              it += elem_size )
                    {
                        auto& elem = *reinterpret_cast<BASE*>( it );
                        f( elem );
                    }
                }
                
                virtual ~polycont_segment_base() {}
                
                virtual std::size_t size() const = 0;
                virtual std::size_t element_size() const = 0;
                virtual unsigned char* begin() = 0;
                virtual const unsigned char* begin() const = 0;
                virtual void insert( const BASE& elem ) = 0;
            };
            
            template<typename T , typename = TURBO_ENABLE_IF( std::is_base_of<BASE,T> )>
            struct polycont_segment : public polycont_segment_base
            {
            private:
                std::vector<T> _elems;
                
                virtual std::size_t size() const override 
                {
                    return _elems.size();
                }
                
                virtual std::size_t element_size() const override
                {
                    return sizeof(T);
                }
                
                virtual const unsigned char* begin() const override
                {
                    return reinterpret_cast<const unsigned char*>( 
                           reinterpret_cast<const BASE*>( _elems.data() )
                           );
                }
                
                virtual unsigned char* begin() override
                {
                    return reinterpret_cast<unsigned char*>( 
                           reinterpret_cast<BASE*>( const_cast<T*>( _elems.data() ) )
                           );
                }
                
                virtual void insert( const BASE& elem ) override
                {
                    _elems.push_back( static_cast<const T&>( elem ) );
                }
            };
            
            using pointer_t   = std::unique_ptr<polycont_segment_base>;
            using key_t       = std::type_index;
            using container_t = std::unordered_map<key_t,pointer_t>;
            
            container_t _segments;
        };
    }
}
