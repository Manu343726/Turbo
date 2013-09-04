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
        template<typename FLAG , typename A , typename B>
        struct _conditional;

        template<typename A , typename B>
        struct _conditional<mpl::true_type,A,B> : public mpl::type_t<A> {};
        
        template<typename A , typename B>
        struct _conditional<mpl::false_type,A,B> : public mpl::type_t<B> {};
    }
    
    template<typename CONDITION , typename A , typename B>
    using conditional = typename _conditional<CONDITION,A,B>::type;
    
    
    template<typename BEGIN , typename END , typename START_RESULT , template<typename,typename> class KERNEL>
    class for_loop
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
}

#endif	/* FOR_HPP */

