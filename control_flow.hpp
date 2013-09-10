/* 
 * File:   for.hpp
 * Author: paradise
 *
 * Created on 4 de septiembre de 2013, 13:46
 */

#ifndef FOR_HPP
#define	FOR_HPP

#include "list.hpp"
#include "basic_types.hpp"

namespace mpl
{
    namespace 
    {
        template<bool FLAG , typename A , typename B>
        struct _conditional;

        template<typename A , typename B>
        struct _conditional<true,A,B> : public mpl::type_t<A> {};
        
        template<typename A , typename B>
        struct _conditional<false,A,B> : public mpl::type_t<B> {};
        
        
        template<typename BEGIN , typename END , typename START_RESULT , template<typename,typename> class KERNEL>
        class __for_loop
        {
        private:
            template<typename CURRENT , typename _END , typename PREVIOUS_RESULT , bool FINISHED>
            struct _for_loop
            {
                using kernel_result = typename KERNEL<typename CURRENT::value , PREVIOUS_RESULT>::result;
                using next = _for_loop<mpl::next<CURRENT> , _END , kernel_result,mpl::equal<mpl::next<CURRENT>,_END>::value>;
                using result = typename next::result;
            };

            template<typename CURRENT , typename _END , typename PREVIOUS_RESULT>
            struct _for_loop<CURRENT,_END,PREVIOUS_RESULT,true>
            {
                using result = PREVIOUS_RESULT;
            };

        public:
            using result = typename _for_loop<BEGIN,END,START_RESULT,mpl::equal<BEGIN,END>::value>::result;
        };

        
        
        template<typename BEGIN , typename END , template<typename> class KERNEL>
        class __for_each
        {
            template<typename CURRENT , bool FINISHED , typename... KERNELS>
            struct _for_each
            {
                using kernel_result = typename KERNEL<typename CURRENT::value>::result;
                using result = typename _for_each<mpl::next<CURRENT>, mpl::equal<mpl::next<CURRENT>,END>::value,KERNELS...,kernel_result>::result;
            };

            template<typename CURRENT , typename... KERNELS>
            struct _for_each<CURRENT , true , KERNELS...>
            {
                using result = mpl::list<KERNELS...>;
            };

        public:
            using result = typename _for_each<BEGIN,false>::result;
        };
    }
    
    template<typename CONDITION , typename A , typename B>
    using conditional = typename _conditional<CONDITION::value,A,B>::type;
    
    template<typename BEGIN , typename END , typename INIT_DATA , template<typename,typename> class KERNEL>
    using for_loop = typename __for_loop<BEGIN,END,INIT_DATA,KERNEL>::result;
    
    template<typename BEGIN , typename END , template<typename> class KERNEL>
    using for_each = typename __for_each<BEGIN,END,KERNEL>::result;
}

#endif	/* FOR_HPP */

