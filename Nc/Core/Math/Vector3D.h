
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

    File Created At:        17/10/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_VECTOR3D_INCLUDED_H_
#define NC_CORE_MATH_VECTOR3D_INCLUDED_H_

#include "Vector.h"

namespace Nc
{
    namespace Math
    {
        /// To manipulate 3 composante of type T
        template<typename T>
        class Vector3D : public Vector<T,3>
        {
            public:
                Vector3D() : Vector<T,3>()  {};
                template<typename U>
                Vector3D(const Vector<U,3> &v) : Vector<T,3>(v) {}
                Vector3D(const T &x, const T &y, const T &z);
                Vector3D(const T v[]);

                template<typename U>
                Vector3D    &operator = (const Vector3D<U> &v);

                /** Initialize the values of the vector */
                void        Init(const T &x = 0, const T &y = 0, const T &z = 0);

                /** Compute the crossProduct with `v` in `result` */
                void        Cross(const Vector3D &v, Vector3D<T> &result) const;

                /** \return X */
                const T     &X() const    {return Vector<T,3>::data[0];}
                /** \return Y */
                const T     &Y() const    {return Vector<T,3>::data[1];}
                /** \return Z */
                const T     &Z() const    {return Vector<T,3>::data[2];}
        };

        template<typename T>
        Vector3D<T>::Vector3D(const T &x, const T &y, const T &z)
        {
            Init(x, y, z);
        }

        template<typename T>
        Vector3D<T>::Vector3D(const T v[])
        {
            Init(v[0], v[1], v[2]);
        }


        template<typename T>
        template<typename U>
        Vector3D<T>    &Vector3D<T>::operator = (const Vector3D<U> &v)
        {
            static_cast<Vector<T,3>*>(this)->operator=(static_cast<Vector<T,3> >(v));
            return *this;
        }

        template<typename T>
        void Vector3D<T>::Init(const T &x, const T &y, const T &z)
        {
            Vector<T,3>::data[0] = x;
            Vector<T,3>::data[1] = y;
            Vector<T,3>::data[2] = z;
        }

        template<typename T>
        void Vector3D<T>::Cross(const Vector3D<T> &v, Vector3D<T> &result) const
        {
            result.data[0] = (Vector<T,3>::data[1] * v.data[2]) - (Vector<T,3>::data[2] * v.data[1]);
            result.data[1] = (Vector<T,3>::data[2] * v.data[0]) - (Vector<T,3>::data[0] * v.data[2]);
            result.data[2] = (Vector<T,3>::data[0] * v.data[1]) - (Vector<T,3>::data[1] * v.data[0]);
        }
    }
}

#endif
