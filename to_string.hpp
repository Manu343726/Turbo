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

#ifndef TO_STRING_HPP
#define	TO_STRING_HPP

#ifndef DISABLE_TURBO_DEPENDENCIES
#include "value_t.hpp"
#endif

#include <string>
#include <sstream>
#include <typeinfo>
#include <iterator>

namespace implementation__demangling
{
#if defined( _MSC_VER )
    #include <Dbghelp.h>

    const std::size_t UNDECORATED_NAME_LENGHT = 512; //No creo que haya nombres mucho más largos

    //MSVC demangling implementation
    std::string demangle(const std::string& name)
    {
        char output_buffer[UNDECORATED_NAME_LENGHT];

        if( !FAILED( UnDecorateSymbolName( name.c_str() , output_buffer , UNDECORATED_NAME_LENGHT , UNDNAME_COMPLETE ) ) )
        {
            return std::string( output_buffer );
        }
        else
            return name;
    }
#endif /* MSVC */

#if defined ( __GNUC__ )

    #include <cxxabi.h>

    //GCC demangling implementation
    std::string demangle(  const std::string& name )
    { 
        int status;

        char* demangled_name = abi::__cxa_demangle( name.c_str() , 0 , 0 , &status );

        if( status != 0 )
            throw;

        std::string result = demangled_name;

        free( demangled_name );

        return result;
    }
#else /* Others... */
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

#ifndef DISABLE_TURBO_DEPENDENCIES
    template<typename T>
    struct to_string_t : public implementation__to_string::_to_string<T,mpl::is_value<T>::value> {};
#else
    template<typename T>
    struct to_string_t : public implementation__to_string::_to_string<T,false> {};
#endif
    

    template<typename T>
    std::string to_string() //Una función en lugar de un functor? Esta es la razón: http://stackoverflow.com/a/7505108/1609356
    {
        return mpl::to_string_t<T>();
    }
}

#endif	/* TO_STRING_HPP */

