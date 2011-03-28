
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

    File Created At:        14/10/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    struct Array<T, D = 0>
    Define an array with the dimension D and data T
    the accessor operator[] is protected by an Exception

    specialisation Array<T, 0> allow dynamic allocation array


    exemple:
    Array<float, 20>    a1;     // array static
    Array<float>        a2(20); // array dynamic

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_ARRAY_INCLUDED_H_
#define NC_CORE_MATH_ARRAY_INCLUDED_H_

#include <string.h>
#include <iostream>
#include "../Define.h"
#include "../Utils/Exception.h"

namespace Nc
{
    namespace Math
    {
        /// Manipulate an array of Dimension D and Type T, This array is static
        template<typename T, unsigned int D = 0>
        struct Array
        {
            /// constructor
            Array() {}
            Array(const Array &a);
            Array &operator = (const Array &a);
            template<unsigned int D2>
            Array(const Array<T,D2> &a);
            template<unsigned int D2>
            Array &operator = (const Array<T,D2> &a);
            virtual ~Array()    {};

            /// initialisation
            void Init(const T &data);

            /// accessor
            inline unsigned int Size()  const {return D;}

            inline T &operator [] (unsigned int i)
            {
                if (i >= D)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            inline const T &operator [] (unsigned int i) const
            {
                if (i >= D)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            friend std::ostream &operator << (std::ostream &os, const Array<T,D> &a)
            {
                for (unsigned int i = 0; i < D; i++)
                    os << a.Data[i] << " ";
                return os;
            }

            // array
            T               Data[D];
        };

        template<typename T, unsigned int D>
        Array<T,D>::Array(const Array<T,D> &a)
        {
            memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()) * sizeof(T));
        }

        template<typename T, unsigned int D>
        Array<T,D> &Array<T,D>::operator = (const Array<T,D> &a)
        {
            memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()) * sizeof(T));
            return *this;
        }

        template<typename T, unsigned int D>
        template<unsigned int D2>
        Array<T,D>::Array(const Array<T,D2> &a)
        {
            memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()) * sizeof(T));
        }

        template<typename T, unsigned int D>
        template<unsigned int D2>
        Array<T,D> &Array<T,D>::operator = (const Array<T,D2> &a)
        {
            memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()) * sizeof(T));
            return *this;
        }

        template<typename T, unsigned int D>
        void Array<T,D>::Init(const T &data)
        {
            for (unsigned int i = 0; i < D; ++i)
                Data[i] = data;
        }

        /// Manipulate an array of Type T, This array is dynamic, It's a simplified alternative to the class std::vector
        template<typename T>
        struct Array<T, 0>
        {
            /// constructor
            Array()
            {
                _size = 0;
                Data = NULL;
            }

            Array(unsigned int size)
            {
                _size = size;
                Data = new T[size];
            }

            virtual ~Array()
            {
                if (Data != NULL)
                    delete[] Data;
            }

            Array(const Array &a)
            {
                _size = a.Size();
                Data = new T[_size];
                memcpy(Data, a.Data, _size * sizeof(T));
            }

            template<typename U, unsigned int D2>
            Array(const Array<U,D2> &a)
            {
                _size = a.Size();
                Data = new T[_size];
                memcpy(Data, a.Data, _size * sizeof(T));
            }

            Array &operator = (const Array &a)
            {
                if (Data != NULL && _size != a._size) // delete si la taille est differente
                    delete[] Data;
                if (Data == NULL || _size != a._size) // malloc si la taille est differente
                    Data = new T[a._size];
                _size = a.Size();
                memcpy(Data, a.Data, _size * sizeof(T));
                return *this;
            }

            template<typename U, unsigned int D2>
            Array &operator = (const Array<U,D2> &a)
            {
                _size = a.Size();
                if (Data != NULL)
                    delete[] Data;
                Data = new T[_size];
                memcpy(Data, a.Data, _size * sizeof(T));
                return *this;
            }

            /// initialisation
            void InitSize(unsigned int s) // init de la taille
            {
                _size = s;
                if (Data != NULL)
                    delete[] Data;
                if (_size > 0)
                    Data = new T[_size];
                else
                    Data = NULL;
            }

            /// don't delete the data, just reset the property _size if s is lesser than _size
            void UnderSize(unsigned int s)    // /!\ to use wisely
            {
                if (s < _size)
                    _size = s;
            }

            void InitData(const T &data) // init de la data
            {
                for (unsigned int i = 0; i < _size; ++i)
                    Data[i] = data;
            }

            void InitData(const T *data) // init de la data avec un array
            {
                memcpy(Data, data, _size * sizeof(T));
            }


            void Resize(unsigned int s)
            {
                T *oldData = Data;

                Data = new T[s];
                if (Data != NULL)
                {
                    memcpy(Data, oldData, ((s <= _size) ? s : _size) * sizeof(T));
                    delete[] oldData;
                }
                _size = s;
            }

            /// accessor
            inline unsigned int Size()  const {return _size;}

            inline T &operator [] (unsigned int i)
            {
                if (i >= _size)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            inline const T &operator [] (unsigned int i) const
            {
                if (i >= _size)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            friend std::ostream &operator << (std::ostream &os, const Array<T,0> &a)
            {
                for (unsigned int i = 0; i < a._size; i++)
                    os << a[i] << " ";
                return os;
            }

            // array
            T               *Data;

            private:
                unsigned int    _size;
        };
    }
}

#endif
