
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        06/12/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_PLANE_H_
#define NC_CORE_MATH_PLANE_H_

#include "Vector3D.h"

namespace Nc
{
    namespace Math
    {
        /// Define a plane by using a Normal and a Point
        template<typename T>
        class Plane
        {
            public:
                Plane()                                                                         {}
                /** Create the plane with the normal and point of the plane */
                Plane(const Vector3D<T> &normal, const Vector3D<T> &point)                      {}
                /** Create the plane with 3 points */
                Plane(const Vector3D<T> &p1, const Vector3D<T> &p2, const Vector3D<T> &p3)      {Init(p1, p2, p3);}

                /** Init the plane with 3 points */
                void    Init(const Vector3D<T> &p1, const Vector3D<T> &p2, const Vector3D<T> &p3);
                /** Init the plane with the normal and point of the plane */
                void    Init(const Vector3D<T> &normal, const Vector3D<T> &point);

                /** \return the distance between the plane and the given point */
                T       Distance(const Vector3D<T> &p);
                /** \return the normal vector of the plane */
                const Vector3D<T> &Normal() const               {return _normal;}
                /** \return the reference point of the plane */
                const Vector3D<T> &Point() const                {return _point;}

            private:
                Vector3D<T>     _normal;        ///< normal witch define the orientation of the plane
                Vector3D<T>     _point;         ///< reference point for the vector
                T               _d;
        };

        template<typename T>
        void Plane<T>::Init(const Vector3D<T> &p1, const Vector3D<T> &p2, const Vector3D<T> &p3)
        {
            Vector3D<T> tmp1(p3 - p2);
            Vector3D<T> tmp2(p1 - p2);
            tmp1.Cross(tmp2, _normal);
            _normal.Normalize();
            _point = p2;
            _d = -(_normal.Dot(_point));
        }

        template<typename T>
        void Plane<T>::Init(const Vector3D<T> &normal, const Vector3D<T> &point)
        {
            _normal = normal;
            _point = point;
            _d = -(_normal.Dot(_point));
        }

        template<typename T>
        T   Plane<T>::Distance(const Vector3D<T> &p)
        {
            return (_d + _normal.Dot(p));
        }
    }
}

#endif // NC_CORE_MATH_PLANE_H_
