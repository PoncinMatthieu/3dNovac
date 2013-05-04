
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        19/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Provide a structure of Pixel 32 bit RGBA

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_PIXEL_H_
#define NC_CORE_MATH_PIXEL_H_

#include <sstream>
#include "../Define.h"

namespace Nc
{
    namespace Math
    {
        /// Define a 4 composant pixel rgba of type T (basicly float or char).
        template<typename T>
        struct  Pixel
        {
            Pixel(const T &red = 0, const T &green = 0, const T &blue = 0, const T &alpha = 1)
                : r(red), g(green), b(blue), a(alpha) {}

            inline void Init(const T &red = 0, const T &green = 0, const T &blue = 0, const T &alpha = 1)    {r = red; g = green; b = blue; a = alpha;}
            inline bool operator == (const Pixel& v) const                                      {return (r == v.r && g == v.g && b == v.b && a == v.a);}
            inline bool operator != (const Pixel& v) const                                      {return (r != v.r || g != v.g || b != v.b || a != v.a);}
            inline T &operator [] (const T &i)                                                  {return ((i == 0) ? r : ((i == 1) ? g : ((i == 2) ? b : a)));}

            inline void operator = (unsigned int v)                                             {r = (v >> 16) & 0xff; g = (v >> 8) & 0xff; b = v & 0xff; a = (v >> 24) & 0xff;}
            inline bool operator == (unsigned int v) const                                      {return (r == ((v >> 16) & 0xff) && g == ((v >> 8) & 0xff) && b == (v & 0xff) && a == ((v >> 24) & 0xff));}
            inline bool operator != (unsigned int v) const                                      {return (r != ((v >> 16) & 0xff) || g != ((v >> 8) & 0xff) || b != (v & 0xff) || a != ((v >> 24) & 0xff));}

            inline Pixel operator + (const Pixel& c) const
            {
                Pixel t = *this;
                t += c;
                return t;
            }

            inline Pixel &operator += (const Pixel& c)
            {
                r += c.r;
                g += c.g;
                b += c.b;
                a += c.a;
                return *this;
            }

            inline Pixel &operator *= (const T &m)
            {
                r *= m;
                g *= m;
                b *= m;
                a *= m;
                return *this;
            }

            friend /*LCORE*/ std::ostream& operator << (std::ostream& os, const Pixel& v)
            {
                os<< "R = " << (unsigned int)v.r << "\tG = " << (unsigned int)v.g << "\tB = " << (unsigned int)v.b << "\tA = " << (unsigned int)v.a;
                return os;
            }

            T   r;      ///< Red componant.
            T   g;      ///< Green componant.
            T   b;      ///< Blue componant.
            T   a;      ///< Alpha componant.
        };

		template struct Pixel<float>;		///< explicit instanciation of a Pixel defined with floating values.
    }
}

#endif
