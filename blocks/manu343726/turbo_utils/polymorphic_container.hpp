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
#include <tuple>

#include "enable_if.hpp"

namespace tml
{
    namespace runtime
    {
        template<typename BASE>
        struct poly_container
        {   
        private:
            struct iterator;
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
            
            iterator begin() const
            {
                return { std::begin( _segments ) , 0 };
            }
            
            iterator end() const
            {
                return { std::end( _segments ) , 0 };
            }
            
            iterator begin()
            {
                return { std::begin( _segments ) , 0 };
            }
            
            iterator end()
            {
                return { std::end( _segments ) , 0 };
            }
        private:
            struct polycont_segment_base
            {
                virtual ~polycont_segment_base() {}
                
                virtual std::size_t size() const = 0;
                virtual std::size_t element_size() const = 0;
                
                virtual unsigned char*       begin() = 0;
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
            
            struct iterator
            {
                using table_iterator = typename container_t::iterator;
            
                iterator( table_iterator it , std::size_t i ) :
                    _table_iterator{ it },
                    _element_index{ i }
                {}
                
                const BASE& operator*() const
                {
                    return *reinterpret_cast<const BASE*>( segment().begin() + ( segment().element_size() * _element_index ) );
                }
                
                BASE& operator*()
                {   
                    return *reinterpret_cast<BASE*>( segment().begin() + ( segment().element_size() * _element_index ) );
                }
                
                iterator& operator++()
                {
                    if( _element_index < segment().size() - 1 )
                    {
                        _element_index++;
                    }
                    else
                    {
                        _table_iterator++;
                        _element_index = 0;
                    }
                    
                    return *this;
                }
                
                iterator& operator++(int)
                {
                    iterator tmp{ *this };
                    
                    ++(*this);
                    
                    return tmp;
                }
                
                friend bool operator==( const iterator& lhs , const iterator& rhs )
                {
                    return std::tie( lhs._table_iterator , lhs._element_index ) == std::tie( rhs._table_iterator , rhs._element_index );
                }
                
                friend bool operator!=( const iterator& lhs , const iterator& rhs )
                {
                    return !(lhs == rhs);
                }
                
                friend bool operator<( const iterator& lhs , const iterator& rhs )
                {
                    return std::tie( lhs._table_iterator , lhs._element_index ) < std::tie( rhs._table_iterator , rhs._element_index );
                }
                
            private:
                const polycont_segment_base& segment() const
                {
                    return *_table_iterator->second;
                }
                
                polycont_segment_base& segment()
                {
                    return *_table_iterator->second;
                }
                
                table_iterator _table_iterator;
                std::size_t    _element_index;
            };
            
            container_t _segments;
        };
    }
}
