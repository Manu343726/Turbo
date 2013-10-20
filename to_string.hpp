/* 
 * File:   to_string.hpp
 * Author: manu343726
 *
 * Created on 9 de septiembre de 2013, 1:32
 */

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

