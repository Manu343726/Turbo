#include <iostream>
#include <functional>
#include <typeinfo>

template<std::size_t COUNT , typename ARG_TYPE , typename... TYPES>
struct function_signature_builder
{
    using type = typename function_signature_builder<COUNT-1,ARG_TYPE,TYPES...,ARG_TYPE>::type;
};

template<std::size_t COUNT , typename ARG_TYPE , typename... TYPES>
struct function_signature_builder<0,ARG_TYPE,TYPES...>
{
    using type = std::function<void(const TYPES&...)>;
};

template<typename VARIABLE , std::size_t COUNT>
using function_type = typename function_signature_builder<COUNT,VARIABLE>::type;

int main() {
    using my_function = function_type<bool,4>;
    
    std::cout << typeid( my_function ).name() << std::endl;
}
