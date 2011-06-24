
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Math.h"
#include "Pixel.h" // to force the compilation

namespace Nc
{
    namespace Math
    {
        Vector3f Normal(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, bool normalize)
        {
            Vector3f v1(p2.Data[0] - p1.Data[0], p2.Data[1] - p1.Data[1], p2.Data[2] - p1.Data[2]);
            Vector3f v2(p3.Data[0] - p1.Data[0], p3.Data[1] - p1.Data[1], p3.Data[2] - p1.Data[2]);
            Vector3f v3;
            v1.Cross(v2, v3);
            if (normalize)
                v3.Normalize();
            return (v3);
        }

        void Normalize(float p[3])
        {
            float len = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
            p[0] /= len;
            p[1] /= len;
            p[2] /= len;
        }

        bool InRect(const Vector2f &RectPos, const Vector2f &RectSize, const Vector2f &pointTest)
        {
            Vector2f posRectSize = RectPos + RectSize;
            if ((pointTest.Data[0] > RectPos.Data[0] && pointTest.Data[0] < posRectSize.Data[0] && posRectSize.Data[0] > RectPos.Data[0]) ||
                (pointTest.Data[0] < RectPos.Data[0] && pointTest.Data[0] > posRectSize.Data[0] && posRectSize.Data[0] < RectPos.Data[0]))
                if ((pointTest.Data[1] > RectPos.Data[1] && pointTest.Data[1] < posRectSize.Data[1] && posRectSize.Data[1] > RectPos.Data[1]) ||
                    (pointTest.Data[1] < RectPos.Data[1] && pointTest.Data[1] > posRectSize.Data[1] && posRectSize.Data[1] < RectPos.Data[1]))
                        return true;
            return false;
        }
    }
}
