/******************************************************************************
* Turbo C++11 metaprogramming Library                                         *
*                                                                             *
* Copyright (C) 2013, Manuel Sánchez Pérez                                    *
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

#pragma once

#ifndef ANALOGLITERALS_HPP
#define ANALOGLITERALS_HPP

#include "operators.hpp"
#include "basic_types.hpp"
#include "expressions.hpp"

namespace mpl
{
    namespace utils
    {
        namespace analog_literals {

        typedef unsigned int uint;

        // Symbols

          enum line_end { o, I };
          enum Lsymbol { L };

        // Intermediary types used during construction

          struct eLsymbol {};
          eLsymbol operator! (Lsymbol) { return eLsymbol(); }

          struct gen { template <typename T> operator T () const { return T(); } };

          template <typename T, uint n> struct excls { excls<T, n + 1> operator! () const { return gen(); } };

          template <typename T, uint n> struct dashes: excls<dashes<T, n>, 0>
          { dashes<T, n + 1> operator-- (int) const { return gen(); } };

          template <typename, uint> struct L_symbols {}; // represents a L|L|L|.. series
          template <typename T, uint n> L_symbols<T, n + 1> operator| (L_symbols<T, n>, Lsymbol) { return gen(); }

          template <typename, uint> struct eL_symbols {}; // represents a !L|!L|!L|.. series
          template <typename T, uint n> eL_symbols<T, n + 1> operator| (eL_symbols<T, n>, eLsymbol) { return gen(); }

          dashes<line_end, 1> operator-- (line_end, int) { return gen(); }
          excls<line_end, 1> operator! (line_end) { return gen(); }

        // Result types

          template <uint len> struct line: L_symbols<line<len>, 0>
          { static uint const length; operator uint () const { return len; } };
          template <uint x, uint y> struct rectangle { static uint const width, height, area; };
          template <uint x, uint y, uint z> struct cuboid { static uint const width, height, depth, volume; };

        // Static members

          template <uint len> uint const line<len>::length = len;

          template <uint x, uint y> uint const rectangle<x, y>::width = x;
          template <uint x, uint y> uint const rectangle<x, y>::height = y;
          template <uint x, uint y> uint const rectangle<x, y>::area = x * y;

          template <uint x, uint y, uint z> uint const cuboid<x, y, z>::width = x;
          template <uint x, uint y, uint z> uint const cuboid<x, y, z>::height = y;
          template <uint x, uint y, uint z> uint const cuboid<x, y, z>::depth = z;
          template <uint x, uint y, uint z> uint const cuboid<x, y, z>::volume = x * y * z;

          template <uint x, uint y, uint z> rectangle<x, y> front (cuboid<x, y, z>) { return gen(); }
          template <uint x, uint y, uint z> rectangle<z, y> side (cuboid<x, y, z>) { return gen(); }
          template <uint x, uint y, uint z> rectangle<x, z> top (cuboid<x, y, z>) { return gen(); }

        // Equality

          template <uint ax, uint bx> bool operator== (line<ax>, line<bx>) { return ax == bx; }

          template <uint ax, uint ay, uint bx, uint by> bool operator== (rectangle<ax, ay>, rectangle<bx, by>)
          { return ax == bx && ay == by; }

          template <uint ax, uint ay, uint az, uint bx, uint by, uint bz>
          bool operator== (cuboid<ax, ay, az>, cuboid<bx, by, bz>) { return ax == bx && ay == by && az == bz; }

        // Construction

          // line

          line<0> operator- (line_end, line_end) { return gen(); }
          template <uint x> line<x> operator- (dashes<line_end, x>, line_end) { return gen(); }

          // rectangle

          template <uint x, uint y> struct lower_rectangle {}; // with lower right corner

          template <uint excl_marks, uint x>
          lower_rectangle<x, (excl_marks + 1) / 2> operator- (excls<dashes<line_end, x>, excl_marks>, line_end)
          { return gen(); }

          template <uint x, uint y> rectangle<x, y> operator| (line<x>, lower_rectangle<x, y>) { return gen(); }

          // cuboid

          template <uint x, uint y, uint z> struct cuboid_top {};
          template <uint x, uint y, uint z> struct half_cuboid {};
            // dimensions of complete cuboid known, rest is for show

          template <uint x, uint n>
          cuboid_top<x, n + 1, n> operator| (L_symbols<line<x>, n>, line<x>) { return gen(); }

          template <uint x, uint y, uint z, uint n>
          eL_symbols<half_cuboid<x, y + (n + 1) / 3, z>, 0> // todo: assert: n%3=2
            operator| (cuboid_top<x, y, z>, excls<line_end, n>) { return gen(); }

          template <uint x, uint y, uint z>
          cuboid<x, y, z> operator| (eL_symbols<half_cuboid<x, y, z>, z>, lower_rectangle<x, 1>) { return gen(); }

        // Arithmetic operators (line):

            template<uint n , mpl::uinteger_type rhs_n>
            line<n*rhs_n> operator*( const line<n>& lhs , mpl::uinteger<rhs_n> rhs )
            {
                return gen();
            }

            template<uint n , mpl::uinteger_type lhs_n>
            line<n*lhs_n> operator*( mpl::uinteger<lhs_n> lhs , const line<n>& rhs )
            {
                return gen();
            }

            template<uint n , mpl::uinteger_type rhs_n>
            line<n/rhs_n> operator/( const line<n>& lhs , mpl::uinteger<rhs_n> rhs )
            {
                return gen();
            } 

            template<mpl::uinteger_type lhs_n , mpl::uinteger_type rhs_n>
            line<lhs_n+rhs_n> operator+( const line<lhs_n>& , const line<rhs_n>& )
            {
                return gen();
            }

            template<mpl::uinteger_type lhs_n , mpl::uinteger_type rhs_n>
            line<lhs_n-rhs_n> operator-( const line<lhs_n>& , const line<rhs_n>& )
            {
                return gen();
            }

        // Arithmetic operators (Rectangle):

            template<uint x , uint y , mpl::uinteger_type rhs_n>
            rectangle<x*rhs_n,y*rhs_n> operator*( const rectangle<x,y>& lhs , mpl::uinteger<rhs_n> rhs )
            {
                return gen();      
            }

            template<uint x , uint y , mpl::uinteger_type lhs_n>
            rectangle<x*lhs_n,y*lhs_n> operator*( mpl::uinteger<lhs_n> lhs , const rectangle<x,y>& rhs )
            {
                return gen();      
            }

            template<uint x , uint y , mpl::uinteger_type rhs_n>
            rectangle<x/rhs_n , y/rhs_n> operator/( const rectangle<x,y>& lhs , mpl::uinteger<rhs_n> rhs )
            {
                return gen();
            }

        // Convenience namespaces that can be "using namespace"'d:

          namespace symbols
          {
            using analog_literals::o;
            using analog_literals::I;
            using analog_literals::L;
          }

          namespace shapes
          {
            using analog_literals::line;
            using analog_literals::rectangle;
            using analog_literals::cuboid;
          }

        } // analog_literals
    } //utils
} //mpl
#endif // header guard

#ifdef ANALOGLITERALS_TEST

int main ()
{
  using namespace analog_literals::symbols;
  using namespace analog_literals::shapes;

  line<3>(I-------I);

  rectangle<2, 3>(o-----o
                  |     !
                  !     !
                  !     !
                  o-----o);

  cuboid<6, 6, 3>(o-------------o
                  |L             \
                  | L             \
                  |  L             \
                  |   o-------------o
                  |   !             !
                  !   !             !
                  o   |             !
                   L  |             !
                    L |             !
                     L|             !
                      o-------------o );

  cuboid<3, 4, 2>(o-------o
                  |L       \
                  | L       \
                  |  o-------o
                  |  !       !
                  o  |       !
                   L |       !
                    L|       !
                     o-------o);
}

#endif // testing
