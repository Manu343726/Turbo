/* 
 * File:   for.hpp
 * Author: paradise
 *
 * Created on 4 de septiembre de 2013, 13:46
 */

#pragma once

#include "boolean_predicates.hpp"


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

        
        
        template<typename BEGIN , typename END , template<typename> class KERNEL , template<typename> class FILTER>
        class __for_each
        {
            template<typename CURRENT , bool FINISHED , typename... KERNELS>
            struct _for_each
            {
                /* 
                 * The idea here is to use lazy instantation of the KERNEL through the filter: If the filter passes (Its evaluated to true with
                 * the current value as parameter) the kernel is instantiated with the current value as argumment (In other words, the computations of this step
                 * of the for loop are performed).
                 * If not, thanks to the lazy instantation provided by wrapping the for_each result, the kernel is not instantiated and the computations are not
                 * performed.
                 */
                
                //The value passes the filter: The kernel is instantiated
                template<bool FILTER_RESULT , typename NON_GLOBAL_SPECIALIZATION_WORKAROUND = mpl::no_type>
                struct _result : public _for_each<mpl::next<CURRENT>, mpl::equal<mpl::next<CURRENT>,END>::value,KERNELS...,typename KERNEL<typename CURRENT::value>::result> {};
                
                //The value desn't pass the filter: The kernel is not instantiated
                template<typename NON_GLOBAL_SPECIALIZATION_WORKAROUND>
                struct _result<false,NON_GLOBAL_SPECIALIZATION_WORKAROUND> : public _for_each<mpl::next<CURRENT>, mpl::equal<mpl::next<CURRENT>,END>::value,KERNELS...> {};
                
                
                using kernel_result = typename KERNEL<typename CURRENT::value>::result;
                using result = typename _result<FILTER<typename CURRENT::value>::value>::result;
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
    
    template<typename BEGIN , typename END , template<typename> class KERNEL , template<typename> class FILTER = mpl::true_predicate>
    using for_each = typename __for_each<BEGIN,END,KERNEL,FILTER>::result;
}

