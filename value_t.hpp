/* 
 * File:   value_t.hpp
 * Author: manu343726
 *
 * Created on 9 de septiembre de 2013, 1:35
 */

#ifndef VALUE_T_HPP
#define	VALUE_T_HPP

namespace implementation__is_value
{
    template<typename T>
    struct _is_value
    {       
    private:
        typedef struct { char c[1]; } yes;
        typedef struct { char c[2]; } no;
        
        template<typename C> static yes test(typename C::is_value_flag*);
        template<typename C> static no  test(...);
    public:
        static const bool value = sizeof(test<T>(0)) == sizeof( yes );
    };
}

namespace mpl
{
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
    using is_value = mpl::value_t<bool,implementation__is_value::_is_value<T>::value>;
}

#endif	/* VALUE_T_HPP */

