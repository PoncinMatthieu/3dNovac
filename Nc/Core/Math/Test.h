
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

    File Created At:        27/11/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_TEST_H_
#define NC_CORE_MATH_TEST_H_

#include "../Define.h"
#include "Vector2D.h"

namespace Nc
{
    namespace Math
    {
        /// Store generic intersection and position function tests
        namespace Test
        {
            // test position 2d
            bool    LCORE   PointInRect(const Vector2i &pointTest, const Vector2i &rectPos, const Vector2i &rectSize);
            bool    LCORE   PointInCircle(const Vector2f &pointTest, const Vector2f &center, float radius);

            // test intersection 2d
            bool    LCORE   IntersectionRectCircle(const Vector2f &RectPos, const Vector2f &RectSize, const Vector2f &circleCenter, float circleRadius);
        }
    }
}

#endif
