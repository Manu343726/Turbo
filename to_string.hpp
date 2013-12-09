/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013, Manuel Sánchez Pérez                                    *
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

#ifndef TO_STRING_HPP
#define	TO_STRING_HPP

#include "value_t.hpp"

#include <string>
#include <sstream>
#include <typeinfo>

namespace implementation__demangling
{
#if defined( _MSC_VER )
    #include <Dbghelp.h>

    const unsigned int UNDECORATED_NAME_LENGHT = 512; //No creo que haya nombres mucho más largos

    //MSVC demangling implementation
    std::string demangle(const std::string& name)
    {
        char output_buffer[UNDECORATED_NAME_LENGHT];

        if( !FAILED( UnDecorateSymbolName( name.c_str() , output_buffer , UNDECORATED_NAME_LENGHT , UNDNAME_COMPLETE ) ) )
        {
            return std::string( output_buffer );
        }
        else
            return std::string( name );
    }
#endif /* MSVC */

#if defined ( __GNUC__ )

    #include <cxxabi.h>

    //GCC demangling implementation
    std::string demangle(  const std::string& name )
    { 
        int status;

        return std::string( abi::__cxa_demangle( name.c_str() , 0 , 0 , &status ) );
    }
#else /* GCC */
    std::string demangle(  const std::string& name )
    { 
        return name;
    }
#endif
}

namespace implementation__to_string
{
    template<typename T , bool FLAG>
    struct _to_string
    {
        operator std::string() const
        {
            return implementation__demangling::demangle( typeid( T ).name() );
        }
    };

    template<typename T>
    struct _to_string<T,true>
    {
        operator std::string() const
        {
            std::ostringstream os;
            os << std::boolalpha << T::value;
            return os.str();
        }
    };
}

namespace mpl
{
    template<typename T>
    struct to_string_t : public implementation__to_string::_to_string<T,mpl::is_value<T>::value> {};
    

    template<typename T>
    std::string to_string() //Una función en lugar de un functor? Esta es la razón: http://stackoverflow.com/a/7505108/1609356
    {
        return mpl::to_string_t<T>();
    }
}

#endif	/* TO_STRING_HPP */

