/* 
 * File:   complex.hpp
 * Author: manu343726
 *
 * Created on 30 de septiembre de 2013, 0:48
 */

#ifndef COMPLEX_HPP
#define	COMPLEX_HPP

namespace math
{
    template<typename REAL , typename IMAGINARY>
    struct complex
    {
        using real      = REAL;
        using imaginary = IMAGINARY;
    };
}

#endif	/* COMPLEX_HPP */

