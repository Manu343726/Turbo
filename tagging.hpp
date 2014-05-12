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

#ifndef TAGGING_HPP
#define	TAGGING_HPP

/*
 * This header defines tagging operations for metafuncion overloading support
 * 
 * There are situation where metafunction specialization could not be achieved
 * through pattern matching. In addition to registry functions and enable_if,
 * tags could be used to achieve the same effect.
 * 
 * The following functions and macros could be used to implement tag-based metafunction
 * overloading, to register a tag to an specified set type (The type becomes part of 
 * the type class defined by the tag), etc. 
 */

namespace tml
{
    
    /*
     * This macro defines a class x as an empty struct tag.
     */
    #define TURBO_DEFINE_TYPECLASS( x ) struct x {};

    /*
     * This registration metafunction checks if a type T belongs to an specified type class.
     * 
     * Turbo type classes are represented by tags to preform metafunction overloading.
     * A type T belongs to a type class C if exists a tml::is_in_class<C,T> specialization
     * which result is tml::true_type.
     * 
     * The macro TURBO_ADD_TYPE_TO_CLASS() is defined to perform that operation easily.
     */
    template<typename CLASS , typename T>
    struct is_in_class : public tml::function<tml::false_type>
    {};
    
    #define TURBO_ADD_TYPE_TO_CLASS( class , type ) template<> struct tml::is_in_class<class,type> : public tml::function<tml::true_type>{}; 
}


#endif	/* TAGGING_HPP */

