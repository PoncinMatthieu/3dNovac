
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

#ifndef NC_CORE_MATH_VECTOR_INCLUDED_H_
#define NC_CORE_MATH_VECTOR_INCLUDED_H_

#include <math.h>
#include <sstream>
#include "../Define.h"
#include "../Utils/Exception.h"

namespace Nc
{
    namespace Math
    {
        /// template class Vector to manipulate a vector of Dimension D of type T
        /**
            template class Vector to manipulate a vector of Dimension D of type T
            operator defined : = [] += -= *= /= + - * / == !=       <br\>
            function :         Lenght / Normalize / Dot / Reverse   <br\>
        */
        template<typename T, unsigned char D>
        class Vector
        {
            public:
                Vector();
                Vector(const Vector &v);
                template<typename U> Vector(const Vector<U,D> &v);
                template<typename U, unsigned char D2> Vector(const Vector<U,D2> &v);
                virtual ~Vector()    {}

                // operator
                T &operator                             []  (unsigned char i);
                const T &operator                       []  (unsigned char i) const;
                Vector &operator                        =   (const Vector &v);
                template<typename U> Vector &operator   =   (const Vector<U,D> &v);
                template<typename U, unsigned char D2> Vector &operator   =   (const Vector<U,D2> &v);
                template<typename U> bool operator      ==  (const Vector<U,D> &v) const;
                template<typename U> bool operator      !=  (const Vector<U,D> &v) const;

                template<typename U> Vector &operator   +=  (const Vector<U,D> &v);

                template<typename U> Vector &operator   -=  (const Vector<U,D> &v);

                template<typename U> Vector &operator   *=  (const Vector<U,D> &v);
                template<typename U> Vector &operator   *=  (const U &a);

                template<typename U> Vector &operator   /=  (const Vector<U,D> &v);
                template<typename U> Vector &operator   /=  (const U &a);

                template<typename U> Vector operator    +   (const Vector<U,D> &v) const;

                template<typename U> Vector operator    -   (const Vector<U,D> &v) const;
                Vector                      operator    -   (void) const;

                template<typename U> Vector operator    *   (const Vector<U,D> &v) const;
                template<typename U> Vector operator    *   (const U &v) const;

                template<typename U> Vector operator    /   (const Vector<U,D> &a) const;
                template<typename U> Vector operator    /   (const U &a) const;

                /** \return true if the given vector is equal with the given precision */
                bool    Equal(const Vector &v, float precision) const;

                // function
                /** \return the square root length of the vector */
                T       Length() const;
                /** scale the vector */
                void    Scale(T l);
                /** Normalize the vector and return itself */
                Vector  &Normalize();
                /** \return the dot product of the vector with \p v */
                template<typename U>
                T       Dot(const Vector<U,D> &v) const;
                /** Fill the inverse vector */
                void    Inverse(Vector &v) const;
                /** \return the angle between the vector and \p v */
                template<typename U>
                T       Angle(const Vector<U,D> &v) const;

                friend /*LCORE*/ std::ostream &operator << (std::ostream &os, const Vector<T,D> &V)
                {
                    char c = 'x';
                    for (unsigned char i = 0; i < D; ++i, ++c)
                        os << c << " = " << V.data[i] << "  ";
                    return os;
                }

                T   data[D];                                ///< public data, for acces performance reason

                static const Vector<T,D>    Null;           ///< static const vector null
        };

        template<typename T, unsigned char D>
        const Vector<T,D>   Vector<T,D>::Null;

        template<typename T, unsigned char D>
        Vector<T,D>::Vector()
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] = 0;
        }

        template<typename T, unsigned char D>
        Vector<T,D>::Vector(const Vector &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] = v.data[i];
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D>::Vector(const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] = v.data[i];
        }

        template<typename T, unsigned char D>
        template<typename U, unsigned char D2>
        Vector<T,D>::Vector(const Vector<U,D2> &v)
        {
            unsigned char i = 0;
            for (; i < D && i < D2; ++i)
                data[i] = v.data[i];
            for (; i < D; ++i)
                data[i] = 0;
        }

        template<typename T, unsigned char D>
        Vector<T,D> &Vector<T,D>::operator = (const Vector &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] = v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator = (const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] = v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U, unsigned char D2>
        Vector<T,D> &Vector<T,D>::operator = (const Vector<U,D2> &v)
        {
            unsigned char i = 0;
            for (; i < D && i < D2; ++i)
                data[i] = v.data[i];
            for (; i < D; ++i)
                data[i] = 0;
            return *this;
        }

        template<typename T, unsigned char D>
        T &Vector<T,D>::operator [] (unsigned char i)
        {
            if (i >= D)
                throw Utils::Exception("Math::Vector", "Overflow in the operator[] of class Vector");
            return data[i];
        }

        template<typename T, unsigned char D>
        const T &Vector<T,D>::operator [] (unsigned char i) const
        {
            if (i >= D)
                throw Utils::Exception("Math::Vector", "Overflow in the operator[] of class Vector");
            return data[i];
        }

        template<typename T, unsigned char D>
        template<typename U>
        bool Vector<T,D>::operator == (const Vector<U,D> &v) const
        {
            for (unsigned char i = 0; i < D; ++i)
                if (data[i] != v.data[i])
                    return false;
            return true;
        }

        template<typename T, unsigned char D>
        template<typename U>
        bool Vector<T,D>::operator != (const Vector<U,D> &v) const
        {
            for (unsigned char i = 0; i < D; ++i)
                if (data[i] != v.data[i])
                    return true;
            return false;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator += (const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] += v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator -= (const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] -= v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator *= (const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] *= v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator /= (const Vector<U,D> &v)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] /= v.data[i];
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator *= (const U &a)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] *= a;
            return *this;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> &Vector<T,D>::operator /= (const U &a)
        {
            for (unsigned char i = 0; i < D; ++i)
                data[i] /= a;
            return *this;
        }


        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator + (const Vector<U,D> &v) const
        {
            Vector r(*this);
            r += v;
            return r;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator - (const Vector<U,D> &v) const
        {
            Vector<T,D> r(*this);
            r -= v;
            return r;
        }

        template<typename T, unsigned char D>
        Vector<T,D> Vector<T,D>::operator - (void) const
        {
            Vector<T,D> r;
            for (unsigned char i = 0; i < D; ++i)
                r.data[i] = -data[i];
            return r;
        }


        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator * (const Vector<U,D> &v) const
        {
            Vector<T,D> r(*this);
            r *= v;
            return r;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator * (const U &a) const
        {
            Vector<T,D> r(*this);
            r *= a;
            return r;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator / (const Vector<U,D> &v) const
        {
            Vector<T,D> r(*this);
            r /= v;
            return r;
        }

        template<typename T, unsigned char D>
        template<typename U>
        Vector<T,D> Vector<T,D>::operator / (const U &a) const
        {
            Vector<T,D> r(*this);
            r /= a;
            return r;
        }

        template<typename T, unsigned char D>
        bool    Vector<T,D>::Equal(const Vector &v, float precision) const
        {
            for (unsigned char i = 0; i < D; ++i)
                if (fabs(data[i] - v.data[i]) > precision)
                    return false;
            return true;
        }

        template<typename T, unsigned char D>
        T Vector<T,D>::Length() const
        {
            T   r = data[0] * data[0];
            for (unsigned int i = 1; i < D; ++i)
                r += data[i] * data[i];
            return sqrt(r);
        }

        template<typename T, unsigned char D>
        void    Vector<T,D>::Scale(T l)
        {
            (*this) *= l;
        }

        template<typename T, unsigned char D>
        Vector<T,D> &Vector<T,D>::Normalize()
        {
            T len = Length();
            if (len != 0)
                (*this) /= Length();
            return (*this);
        }

        template<typename T, unsigned char D>
        template<typename U>
        T   Vector<T,D>::Dot(const Vector<U,D> &u) const
        {
            T   r = data[0] * u.data[0];
            for (unsigned char i = 1; i < D; ++i)
                r += data[i] * u.data[i];
            return r;
        }

        template<typename T, unsigned char D>
        void  Vector<T,D>::Inverse(Vector<T,D> &v) const
        {
            for (unsigned char i = 0; i < D; ++i)
                v.data[i] = -data[i];
        }

        template<typename T, unsigned char D>
        template<typename U>
        T   Vector<T,D>::Angle(const Vector<U,D> &v) const
        {
            T len1 = Length();
            T len2 = v.Length();
            if (len1 != 0 && len2 != 0)
                return acos(Dot(v) / (len1 * len2));
            else
                return 0;
        }
    }
}

#endif
