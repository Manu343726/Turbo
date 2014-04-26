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

#ifndef DEMANGLE_HPP
#define	DEMANGLE_HPP

#include <string>
#include <sstream>
#include <typeinfo>
#include <iterator>
#include <cstdlib>

/*
 * This header provides the tml::impl::demangle() function, a portable 
 * function to apply demangling to symbol names.
 *
 * Currently there are only implemented alternatives for the Microsoft
 * MSVC compiler and the GNU GCC compiler. LLVM/Clang will be added in the 
 * future.
 */

namespace tml{
namespace impl{

#if defined( _MSC_VER ) //MSVC tml::impl::demangle() version
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
#endif

#if defined ( __GNUC__ ) //GCC tml::impl::demangle() version

    #include <cxxabi.h>

    //GCC demangling implementation
    std::string demangle(  const std::string& name )
    { 
        int status;

        char* demangled_name = abi::__cxa_demangle( name.c_str() , 0 , 0 , &status );

        if( status != 0 )
            throw;

        std::string result{ demangled_name };

        std::free( demangled_name ); //VERY IMPORTANT! The result of __cxa_demangle() was malloc()ed, should be released
                                     //using free(). See __cxa_demangle() documentation.
                                
        return result;
    }
#else /* Others (Currently unsupported platforms. The name is returned unchanged) */
    std::string demangle(  const std::string& name )
    { 
        return name;
    }
#endif

}} /* close braces of tml::impl namespace */

#endif	/* DEMANGLE_HPP */

