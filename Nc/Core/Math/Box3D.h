
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

    File Created At:        23/10/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_BOX3D_INCLUDED_H_
#define NC_CORE_MATH_BOX3D_INCLUDED_H_

#include "Box.h"
#include "Vector3D.h"

namespace Nc
{
    namespace Math
    {
        /// To manipulate a box in 3 Dimension of type T
        template<typename T>
        class Box3D : public Box<T,3>
        {
            public:
                Box3D() : Box<T,3>()    {}
                template<typename U>
                Box3D(const Box<U,3> &b) : Box<T,3>(b)  {}
                Box3D(const Vector3f &min, const Vector3f &max) : Box<T,3>(min, max)    {}
                Box3D(const T &xmin, const T &ymin, const T &zmin, const T &xmax, const T &ymax, const T &zmax);

                template<typename U>
                Box3D    &operator = (const Box3D<U> &b);

                static const Box3D<T>       EmptyBox;       ///< static const empty box
        };

        template<typename T>
        const Box3D<T>      Box3D<T>::EmptyBox;

        template<typename T>
        Box3D<T>::Box3D(const T &xmin, const T &ymin, const T &zmin, const T &xmax, const T &ymax, const T &zmax)
        {
            Box<T,3>::_min.data[0] = xmin;
            Box<T,3>::_max.data[0] = xmax;
            Box<T,3>::_min.data[1] = ymin;
            Box<T,3>::_max.data[1] = ymax;
            Box<T,3>::_min.data[2] = zmin;
            Box<T,3>::_max.data[2] = zmax;
        }

        template<typename T>
        template<typename U>
        Box3D<T>    &Box3D<T>::operator = (const Box3D<U> &v)
        {
            static_cast<Box<T,3>*>(this)->operator=(static_cast<Box<T,3> >(v));
            return *this;
        }
    }
}

#endif
