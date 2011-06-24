
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

#ifndef NC_CORE_MATH_VECTOR2D_INCLUDED_H_
#define NC_CORE_MATH_VECTOR2D_INCLUDED_H_

#include "Vector.h"

namespace Nc
{
    namespace Math
    {
        /// Vector 2d to manipulate 2 composante of type T
        template<typename T>
        class Vector2D : public Vector<T,2>
        {
            public:
                Vector2D() : Vector<T,2>()                      {}
                template<typename U>
                Vector2D(const Vector<U,2> &v) : Vector<T,2>(v) {}
                template<typename U>
                Vector2D(const Vector3D<U> &v) : Vector<T,2>(v) {}
                Vector2D(const T &x, const T &y);

                template<typename U>
                Vector2D    &operator = (const Vector2D<U> &v);

                /** Initialize the values of the vector */
                void Init(const T &u = 0, const T &v = 0);

                /** \return X */
                const T   &X() const    {return Vector<T,2>::Data[0];}
                /** \return Z */
                const T   &Y() const    {return Vector<T,2>::Data[1];}
        };

        template<typename T>
        Vector2D<T>::Vector2D(const T &x, const T &y)
        {
             Init(x, y);
        }

        template<typename T>
        template<typename U>
        Vector2D<T>    &Vector2D<T>::operator = (const Vector2D<U> &v)
        {
            static_cast<Vector<T,2>*>(this)->operator=(static_cast<Vector<T,2> >(v));
            return *this;
        }

        template<typename T>
        void Vector2D<T>::Init(const T &u, const T &v)
        {
            Vector<T,2>::Data[0] = u;
            Vector<T,2>::Data[1] = v;
        }
    }
}

#endif
