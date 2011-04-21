
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

    File Created At:        26/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_MATH_H_INCLUDED
#define NC_CORE_MATH_MATH_H_INCLUDED

#include <sstream>
#include <cstdlib>
#include <math.h>

#include "../Define.h"
#include "Array.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"
#include "Box3D.h"
#include "Box2D.h"
#include "Pixel.h"

/// Global Namespace of 3dNovac
namespace Nc
{
    /// Namespace to Manipulate objects like Vector and Matrix and provide some mathematic function
    namespace Math
    {
        // Calculs 3d
        Vector3f LCORE Normal(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, bool normalize = true);
        void     LCORE Normalize(float p[3]);

        // Calculs 2d
        bool     LCORE InRect(const Vector2f &RectPos, const Vector2f &RectSize, const Vector2f &pointTest);

        /** Init the rand to call srand() function */
        inline void     InitRand()                          {srand((unsigned int)time(0));}

        /** return a random number of the type T */
        template<typename T> T          Rand(const T &min, const T &max)                {return min + ((T)rand() / ((float)RAND_MAX + 1) * max);}

        /** return the absolute value */
        template<typename T> T          Abs(const T &v)                                 {return ((v > 0) ? v : -v);}

        /** return true if v1 and v2 are equal with a precision */
        template<typename T> bool       Equal(const T &v1, const T &v2, const T &p)     {return ((v1 >= (v2 - p)) && (v1 <= (v2 + p)));}

        /** return true if v1 is superior of v2 with a precision */
        template<typename T> bool       Sup(const T &v1, const T &v2, const T &p)       {return (v1 > (v2 + p));}

        /** return true if v1 is inferior of v2 with a precision */
        template<typename T> bool       Inf(const T &v1, const T &v2, const T &p)       {return (v1 < (v2 - p));}

        /** swap to element of type T */
        template <typename T>
        void Swap(T &v1, T &v2)
        {
            T tmp = v2;
            v2 = v1;
            v1 = tmp;
        }

        /** return true if the number is a power of 2 */
        template<typename T> bool       IsAPowerOf2(const T &v)
        {
            if (v == 0)
                return false;
            T tmp = v;
            while(((tmp = tmp >> 1) & 1) == 0);
            return ((tmp >> 1) == 0);
        }

        /** return the nearest power of 2 */
        template<typename T> T       NearestPowerOf2(const T &v)
        {
            T   pow = 1;
            for (; pow < v; pow <<= 2);
            return pow;
        }
    }
}

#endif // MMATH_H_INCLUDED
