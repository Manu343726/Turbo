/* 
 * File:   basic_traits.hpp
 * Author: manu343726
 *
 * Created on 1 de septiembre de 2013, 14:08
 */

#ifndef BASIC_TRAITS_HPP
#define	BASIC_TRAITS_HPP

#include <cstddef>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <sstream>

#include "traits.hpp"

//Question here: http://stackoverflow.com/questions/18619360/using-stdfunction-with-union-in-c11

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
#endif /* GCC */
}

namespace make_type_macro
{
    #define MAKE_TYPE(name , type) template<type VALUE> \
                                   using name = mpl::value_t<type,VALUE>; \
                                   using name##_type = type
}

namespace mpl
{
    struct no_type{};
    
    
    template<typename T , T val>
    struct value_t
    {
    private:
        struct _is_a_value {};
    public:
        static const T value = val;
        using value_type = T;
        
        using is_value_flag = _is_a_value;
        
        constexpr T operator()() { return val; }
    }; 
    
    template<typename T>
    struct type_t
    {
        using type = T;
    };
    
    
    template<typename T , typename U>
    struct pair
    {
        using first  = T;
        using second = U;
    };
    
    MAKE_TYPE(ucharacter         , unsigned char);
    MAKE_TYPE(character          , char);
    MAKE_TYPE(uinteger           , unsigned int);
    MAKE_TYPE(integer            , int);
    MAKE_TYPE(ulong_integer      , unsigned long int);
    MAKE_TYPE(long_integer       , long int);
    MAKE_TYPE(ulong_long_integer , unsigned long long int);
    MAKE_TYPE(long_long_integer  , long long int);
    MAKE_TYPE(boolean            , bool);
    MAKE_TYPE(size_t             , std::size_t);
    
    template<typename T>
    struct zero;
    
    template<typename T>
    struct one;
    
    template<typename T , T v>
    struct zero<mpl::value_t<T,v>> : public mpl::value_t<T,0> {};
    
    template<typename T , T v>
    struct one<mpl::value_t<T,v>> : public mpl::value_t<T,1> {};
    
    using false_type = mpl::boolean<false>;
    using true_type  = mpl::boolean<true>;
    
    
    namespace implementation__is_value
    {
        template<typename T>
        struct _is_value
        {       
        private:
            template<typename C> static mpl::true_type  test(typename C::is_value_flag*);
            template<typename C> static mpl::false_type test(...);
        public:
            static const bool value = decltype(test<T>(0))::value;
        };
    }
    
    template<typename T>
    using is_value = mpl::boolean<implementation__is_value::_is_value<T>::value>;
    
    
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
    
    
    template<typename T>
    struct to_string_t : public implementation__to_string::_to_string<T,mpl::is_value<T>::value> {};
    

    template<typename T>
    struct is_value_t : public mpl::false_type {};
    
    template<typename T , T VALUE>
    struct is_value_t<mpl::value_t<T,VALUE>> : public mpl::true_type {};
    
    
    template<typename T>
    struct is_type_t : public mpl::false_type {};
    
    template<typename T>
    struct is_type_t<mpl::type_t<T>> : public mpl::true_type {};
    

    template<typename T>
    std::string to_string() //Una función en lugar de un functor? Esta es la razón: http://stackoverflow.com/a/7505108/1609356
    {
        return mpl::to_string_t<T>();
    }
    
    template<bool b>
    struct to_string_t<mpl::boolean<b>>
    {
        constexpr operator std::string() const
        {
            return b ? "true" : "false";
        }
    };
}

#endif	/* BASIC_TRAITS_HPP */

