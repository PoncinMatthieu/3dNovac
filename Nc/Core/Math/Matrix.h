
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

#ifndef NC_CORE_MATH_MATRIX_INCLUDED_H_
#define NC_CORE_MATH_MATRIX_INCLUDED_H_

#include <iostream>
#include "../Define.h"
#include "../Utils/Exception.h"

namespace Nc
{
    namespace Math
    {
        /// template class to manipulate matrix of type T with nb line D1 and row D2.
        template<typename T, unsigned int D1, unsigned int D2>
        class Matrix
        {
            public:
                // constructor
                Matrix();
                Matrix(const Matrix &m);
                Matrix &operator = (const Matrix &m);
                virtual ~Matrix()   {};

                // ascessor
                inline T const *operator [] (unsigned int i) const
                {
                    if (i >= D1)
                        throw Utils::Exception("Math::Matrix", "Overflow in the operator[] of class Array");
                    return _data + (i * D2);
                }

                inline T *operator [] (unsigned int i)
                {
                    if (i >= D1)
                        throw Utils::Exception("Math::Matrix", "Overflow in the operator[] of class Array");
                    return _data + (i * D2);
                }

                friend /*LCORE*/ std::ostream &operator << (std::ostream &os, const Matrix<T, D1, D2> &m)
                {
                    unsigned char k = 0;
                    for (unsigned char i = 0; i < D1; i++)
                    {
                        for (unsigned char j = 0; j < D2; j++)
                            os << m._data[k++] << " ";
                        os << std::endl;
                    }
                    return os;
                }

                /** \return the elements array of the matrix. */
                inline const T *Elements() const        {return _data;}

                /** Transpose the matrix into \p t. */
                void    Transpose(Matrix<T, D2, D1> &t) const;

            protected:
                unsigned char   _sizeMax;           ///< size of the matrix.
                T               _data[D1 * D2];     ///< array with store the elements of the matrix (lines are D1 and columns are D2).
        };

        template<typename T, unsigned int D1, unsigned int D2>
        Matrix<T,D1,D2>::Matrix()
        {
            _sizeMax = D1 * D2;
            memset(_data, 0, _sizeMax * sizeof(T));
        }

        template<typename T, unsigned int D1, unsigned int D2>
        Matrix<T,D1,D2>::Matrix(const Matrix<T,D1,D2> &m)
        {
            _sizeMax = D1 * D2;
            memcpy(_data, m._data, _sizeMax * sizeof(T));
        }

        template<typename T, unsigned int D1, unsigned int D2>
        Matrix<T,D1,D2> &Matrix<T,D1,D2>::operator = (const Matrix &m)
        {
            _sizeMax = D1 * D2;
            memcpy(_data, m._data, _sizeMax * sizeof(T));
            return *this;
        }

        template<typename T, unsigned int D1, unsigned int D2>
        void    Matrix<T,D1,D2>::Transpose(Matrix<T, D2, D1> &t) const
        {
            for (unsigned char i = 0; i < D1; i++)
            {
                for (unsigned char j = 0; j < D2; j++)
                    t._data[(j * D2) + i] = _data[(i * D1) + j];
            }
        }
    }
}

#endif
