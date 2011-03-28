
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
        template<typename T>
        struct  Pixel
        {
            Pixel(const T &r = 0, const T &g = 0, const T &b = 0, const T &a = 0) : R(r), G(g), B(b), A(a) {}
            ~Pixel() {}

            inline void Init(const T &r = 0, const T &g = 0, const T &b = 0, const T &a = 0)    {R = r; G = g; B = b; A = a;}

            inline bool operator == (const Pixel& v)
            {
                return !(R != v.R || G != v.G || B != v.B || A != v.A);
            }

            inline bool operator != (const Pixel& v)
            {
                return (R != v.R || G != v.G || B != v.B || A != v.A);
            }

            inline Pixel operator + (const Pixel& c) const
            {
                Pixel t = *this;
                t += c;
                return t;
            }

            inline Pixel &operator += (const Pixel& c)
            {
                R += c.R;
                G += c.G;
                B += c.B;
                A += c.A;
                return *this;
            }

            inline Pixel &operator *= (const T &a)
            {
                R *= a;
                G *= a;
                B *= a;
                A *= a;
                return *this;
            }

            inline T &operator [] (const T &i)
            {
                return ((i == 0) ? R : ((i == 1) ? G : ((i == 2) ? B : A)));
            }

            friend std::ostream& operator << (std::ostream& os, const Pixel& v)
            {
                os<< "R = " << v.R << "\tG = " << v.G << "\tB = " << v.B << "\tA = " << v.A;
                return os;
            }

            // 0 to 255
            T   R;
            T   G;
            T   B;
            T   A;    // o for full transparency, 255 for full opacity
        };
    }
}

#endif
