
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

#ifndef NC_CORE_MATH_BOX_INCLUDED_H_
#define NC_CORE_MATH_BOX_INCLUDED_H_

#include "../Utils/Exception.h"
#include "Vector.h"

namespace Nc
{
    namespace Math
    {
        /// Template class to manipulate a box of type T and Dimension D.
        /**
            A box is defined by 2 extrema Vector<T,D> : Min and Max
        */
        template<typename T, unsigned char D>
        class Box
        {
            public:
                Box()   {}
                Box(const Vector<T,D> &min, const Vector<T,D> &max);
                template<typename U>    Box(const Box<U,D> &b);
                virtual ~Box()    {}

                // operator
                template<typename U>    Box &operator   =   (const Box<U,D> &b);
                template<typename U>    Box &operator   +=  (const Box<U,D> &b);
                template<typename U>    Box &operator   +=  (const Vector<U,D> &p);
                template<typename U>    Box &operator   +=  (const U coord[]);
                template<typename U>    Box operator    +   (const Box<U,D> &b) const;

                // accessor
                /** \return the minima point of the box */
                inline const Vector<T,D>    &Min() const               {return _min;}
                /** \return the maxima point of the box */
                inline const Vector<T,D>    &Max() const               {return _max;}
                /** \return the minima coordinate of the given \p dimension */
                const T                     &Min(unsigned char dimension) const;
                /** \return the maxima coordinate of the given \p dimension */
                const T                     &Max(unsigned char dimension) const;
                /** \return the minima coordinate of the given \p dimension */
                void                        Min(unsigned char dimension, const T &v);
                /** \return the maxima coordinate of the given \p dimension */
                void                        Max(unsigned char dimension, const T &v);

                /** \return the center of the box */
                Vector<T,D>                 Center() const;
                /** \return the lenght of the box for each dimensions */
                Vector<T,D>                 Length() const;
                /** \return the lenght of the given dimension index */
                T                           Length(unsigned char dimension) const;
                /** \return the size of the box for each dimension */
                Vector<T,D>                 Size() const;
                /** \return the are of the box */
                T                           Area() const;

                // modifier
                /** Initialize the box */
                void                        Init();
                /** Initialize the box with the given center and size */
                void                        Init(const Vector<T,D> &center, const T &size);
                /** Transform the box with the given matrix */
                void                        Transform(const TMatrix &m);

                /** Print the Min and Max vector of the box */
                friend std::ostream &operator << (std::ostream &os, const Box<T,D> &V)
                {
                    os << "Min: " << V._min << std::endl;
                    os << "Max: " << V._max << std::endl;
                    return os;
                }

            protected:
                Vector<T,D>     _min;       ///< minima of the box
                Vector<T,D>     _max;       ///< maxima of the box
        };

        template<typename T, unsigned char D>
        Box<T,D>::Box(const Vector<T,D> &min, const Vector<T,D> &max)
        {
            _min = min;
            _max = max;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D>::Box(const Box<U,D> &b)
        {
            _min = b._min;
            _max = b._max;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D> &Box<T,D>::operator   =   (const Box<U,D> &b)
        {
            _min = b._min;
            _max = b._max;
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D> &Box<T,D>::operator   +=  (const Box<U,D> &b)
        {
            for (unsigned char i = 0; i < D; ++i)
            {
                if (_min.Data[i] > b._min.Data[i])
                    _min.Data[i] = b._min.Data[i];
                if (_max.Data[i] < b._max.Data[i])
                    _max.Data[i] = b._max.Data[i];
            }
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D> &Box<T,D>::operator   +=  (const Vector<U,D> &p)
        {
            for (unsigned char i = 0; i < D; ++i)
            {
                if (_min.Data[i] > p.Data[i])
                    _min.Data[i] = p.Data[i];
                if (_max.Data[i] < p.Data[i])
                    _max.Data[i] = p.Data[i];
            }
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D> &Box<T,D>::operator += (const U coord[])
        {
            if (_min.Data[0] > coord[0])
                _min.Data[0] = coord[0];
            if (_min.Data[1] > coord[1])
                _min.Data[1] = coord[1];
            if (_min.Data[2] > coord[2])
                _min.Data[2] = coord[2];

            if (_max.Data[0] < coord[0])
                _max.Data[0] = coord[0];
            if (_max.Data[1] < coord[1])
                _max.Data[1] = coord[1];
            if (_max.Data[2] < coord[2])
                _max.Data[2] = coord[2];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Box<T,D> Box<T,D>::operator    +   (const Box<U,D> &b) const
        {
            Box<T,D>    box(*this);
            box += b;
            return box;
        }

        template<typename T, unsigned char D>
        const T   &Box<T,D>::Min(unsigned char dimension) const
        {
            if (dimension >= D)
                throw Utils::Exception("Math::Box", "Overflow in an accesseur of class Box");
            return _min.Data[dimension];
        }

        template<typename T, unsigned char D>
        const T   &Box<T,D>::Max(unsigned char dimension) const
        {
            if (dimension >= D)
                throw Utils::Exception("Math::Box", "Overflow in an accesseur of class Box");
            return _max.Data[dimension];
        }

        template<typename T, unsigned char D>
        void    Box<T,D>::Min(unsigned char dimension, const T &v)
        {
            if (dimension >= D)
                throw Utils::Exception("Math::Box", "Overflow in an accesseur of class Box");
            _min.Data[dimension] = v;
        }

        template<typename T, unsigned char D>
        void    Box<T,D>::Max(unsigned char dimension, const T &v)
        {
            if (dimension >= D)
                throw Utils::Exception("Math::Box", "Overflow in an accesseur of class Box");
            _max.Data[dimension] = v;
        }

        template<typename T, unsigned char D>
        Vector<T,D>     Box<T,D>::Length() const
        {
            return (_max - _min);
        }

        template<typename T, unsigned char D>
        T   Box<T,D>::Length(unsigned char dimension) const
        {
            if (dimension >= D)
                throw Utils::Exception("Math::Box", "Overflow in an accesseur of class Box");
            return _max.Data[dimension] - _min.Data[dimension];
        }

        template<typename T, unsigned char D>
        T   Box<T,D>::Area() const
        {
            T   r = Length(0);

            for (unsigned int i = 1; i < D; ++i)
                r *= Length(i);
            return r;
        }

        template<typename T, unsigned char D>
        Vector<T,D> Box<T,D>::Center() const
        {
            return ((_min + _max) / 2);
        }

        template<typename T, unsigned char D>
        Vector<T,D> Box<T,D>::Size() const
        {
            Vector<T,D> v;
            for (unsigned char i = 0; i < D; ++i)
                v.Data[i] = Length(i);
            return v;
        }

        template<typename T, unsigned char D>
        void Box<T,D>::Transform(const TMatrix &m)
        {
            m.Transform(_min);
            m.Transform(_max);
        }

        template<typename T, unsigned char D>
        void Box<T,D>::Init()
        {
            _min = Vector<T,D>::Null;
            _max = Vector<T,D>::Null;
        }

        template<typename T, unsigned char D>
        void Box<T,D>::Init(const Vector<T,D> &center, const T &size)
        {
            for (unsigned int i = 0; i < D; ++i)
            {
                _min.Data[i] = center.Data[i] - size;
                _max.Data[i] = center.Data[i] + size;
            }
        }

    }
}

#endif
