
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

#include "../Utils/Utils.h"
#include "Test.h"

namespace Nc
{
    namespace Math
    {
        namespace Test
        {
            bool    PointInRect(const Vector2i &pointTest, const Vector2i &rectPos, const Vector2i &rectSize)
            {
                Vector2f posRectSize = rectPos + rectSize;
                if ((pointTest.data[0] > rectPos.data[0] && pointTest.data[0] < posRectSize.data[0] && posRectSize.data[0] > rectPos.data[0]) ||
                    (pointTest.data[0] < rectPos.data[0] && pointTest.data[0] > posRectSize.data[0] && posRectSize.data[0] < rectPos.data[0]))
                    if ((pointTest.data[1] > rectPos.data[1] && pointTest.data[1] < posRectSize.data[1] && posRectSize.data[1] > rectPos.data[1]) ||
                        (pointTest.data[1] < rectPos.data[1] && pointTest.data[1] > posRectSize.data[1] && posRectSize.data[1] < rectPos.data[1]))
                            return true;
                return false;
            }

            bool    PointInCircle(const Vector2f &pointTest, const Vector2f &center, float radius)
            {
                Vector2f centerToTest = center - pointTest;
                return (centerToTest.Length() < radius);
            }

            bool    IntersectionRectCircle(const Vector2f &rectPos, const Vector2f &rectSize, const Vector2f &circleCenter, float circleRadius)
            {
                // compute the distance between the center of the circle and the center of the rectangle
                Vector2f rectSizePer2 = rectSize / 2.f;
                Vector2f circleDistance(abs(circleCenter[0] - (rectPos[0] + rectSizePer2[0])), abs(circleCenter[1] - (rectPos[1] + rectSizePer2[1])));

                // if the circle is too far to intersect
                if (circleDistance[0] > ((rectSizePer2[0]) + circleRadius))
                    return false;
                if (circleDistance[1] > ((rectSizePer2[1]) + circleRadius))
                    return false;

                // if the circle is too close not to intersect
                if (circleDistance[0] <= (rectSizePer2[0]))
                    return true;
                if (circleDistance[1] <= (rectSizePer2[1]))
                    return true;

                // special case, when the circle is at the limit
                return (((circleDistance[0] - (rectSizePer2[0])) * (circleDistance[0] - (rectSizePer2[0])) +
                         (circleDistance[1] - (rectSizePer2[1])) * (circleDistance[1] - (rectSizePer2[1]))) <=
                        (circleRadius * circleRadius));
            }
        }
    }
}
