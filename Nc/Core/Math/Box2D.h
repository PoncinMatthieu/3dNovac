
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

#ifndef NC_CORE_MATH_BOX2D_INCLUDED_H_
#define NC_CORE_MATH_BOX2D_INCLUDED_H_

#include "Box.h"
#include "Vector2D.h"

namespace Nc
{
    namespace Math
    {
        /// To manipulate a box in 2 Dimension of type T.
        template<typename T>
        struct Box2D : public Box<T,2>
        {
            Box2D() : Box<T,2>()    {}
            template<typename U>
            Box2D(const Box<U,2> &b) : Box<T,2>(b)  {}
            Box2D(const Vector2D<T> &min, const Vector2D<T> &max);
            Box2D(const T &xmin, const T &ymin, const T &xmax, const T &ymax);

            template<typename U>
            Box2D    &operator = (const Box2D<U> &b);

            static const Box2D<T>       EmptyBox;       ///< static const empty box.
        };

        template<typename T>
        const Box2D<T>      Box2D<T>::EmptyBox;

        template<typename T>
        Box2D<T>::Box2D(const Vector2D<T> &min, const Vector2D<T> &max)
        {
            Box<T,2>::_min = min;
            Box<T,2>::_max = max;
        }

        template<typename T>
        Box2D<T>::Box2D(const T &xmin, const T &ymin, const T &xmax, const T &ymax)
        {
            Box<T,2>::_min.data[0] = xmin;
            Box<T,2>::_max.data[0] = xmax;
            Box<T,2>::_min.data[1] = ymin;
            Box<T,2>::_max.data[1] = ymax;
        }

        template<typename T>
        template<typename U>
        Box2D<T>    &Box2D<T>::operator = (const Box2D<U> &v)
        {
            static_cast<Box<T,2>*>(this)->operator=(static_cast<Box<T,2> >(v));
            return *this;
        }
    }
}

#endif
