
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

#ifndef NC_CORE_MATH_ARRAY_INCLUDED_H_
#define NC_CORE_MATH_ARRAY_INCLUDED_H_

#include <string.h>
#include <iostream>
#include "../Define.h"
#include "../Utils/Utils.h"

namespace Nc
{
    namespace Math
    {
        /// Manipulate an array of Dimension D and Type T, This array is static, but the specialisation with "D = 0" is dynamic
        template<typename T, unsigned int D = 0>
        struct Array
        {
            // constructor
            Array() {}
            Array(const Array &a);
            Array &operator = (const Array &a);
            template<unsigned int D2>
            Array(const Array<T,D2> &a);
            template<unsigned int D2>
            Array &operator = (const Array<T,D2> &a);
            virtual ~Array()    {};

            // initialisation
            /** Init all node of the array with the given data */
            void Init(const T &data);

            // accessor
            /** \return the size of the array */
            inline unsigned int Size()  const   {return D;}

            /** The accessor will throw an exception if the given index \p i is too long */
            inline T &operator [] (unsigned int i)
            {
                if (i >= D)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            /** The accessor will throw an exception if the given index \p i is too long */
            inline const T &operator [] (unsigned int i) const
            {
                if (i >= D)
                    throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
                return Data[i];
            }

            friend /*LCORE*/ std::ostream &operator << (std::ostream &os, const Array<T,D> &a)
            {
                for (unsigned int i = 0; i < D; ++i)
                    os << a.Data[i] << " ";
                return os;
            }

            // array
            T               Data[D];            ///< data of type T. Could be directly accessed insteed of using the operator []
        };

        /// Manipulate an array of Type T, This array is dynamic, It's a simplified alternative to the class std::vector
        template<typename T>
        struct Array<T, 0>
        {
            // constructor
            Array();
            Array(unsigned int size);
            Array(const Array &a);
            template<typename U, unsigned int D2>
            Array(const Array<U,D2> &a);
            Array &operator = (const Array &a);
            template<typename U, unsigned int D2>
            Array &operator = (const Array<U,D2> &a);
            virtual ~Array();

            // initialisation
            /**
                Init the size of the array with the given size \p s.
                Realocate the array, but no recopy before.
            */
            void InitSize(unsigned int s);

            /**
                Don't delete the data, just reset the property _size if \p s is lesser than _size
                \warning to use wisely. This function could be dangerous
            */
            void UnderSize(unsigned int s);

            /** Init all node of the array with the given data */
            void InitData(const T &data);
            /**
                Init all node of the array with the given array
                \warning the \p data is an array and it shoud be allocated of the size of our Array
            */
            void InitData(const T *data);
            /**
                Resize the container by creating a new one and recopying the datas of the previous one.
            */
            void Resize(unsigned int s);

            // accessor
            /** \return the size of the array */
            inline unsigned int Size()  const   {return _size;}

            /** Accessor with a protection with exception */
            inline T &operator [] (unsigned int i);

            /** Accessor with a protection with exception */
            inline const T &operator [] (unsigned int i) const;

            friend /*LCORE*/ std::ostream &operator << (std::ostream &os, const Array<T,0> &a)
            {
                for (unsigned int i = 0; i < a._size; ++i)
                    os << a.Data[i] << " ";
                return os;
            }

            // array
            T               *Data;      ///< data of type T Dynamically allocated. Could be directly accessed insteed of using the operator [].

            private:
                unsigned int    _size;  ///< The size of the array
        };
    }

    namespace Math
    {
        template<typename T, unsigned int D>
        Array<T,D>::Array(const Array<T,D> &a)
        {
            Utils::Memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()));
        }

        template<typename T, unsigned int D>
        Array<T,D> &Array<T,D>::operator = (const Array<T,D> &a)
        {
            Utils::Memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()));
            return *this;
        }

        template<typename T, unsigned int D>
        template<unsigned int D2>
        Array<T,D>::Array(const Array<T,D2> &a)
        {
            Utils::Memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()));
        }

        template<typename T, unsigned int D>
        template<unsigned int D2>
        Array<T,D> &Array<T,D>::operator = (const Array<T,D2> &a)
        {
            Utils::Memcpy(Data, a.Data, ((D <= a.Size()) ? D : a.Size()));
            return *this;
        }

        template<typename T, unsigned int D>
        void Array<T,D>::Init(const T &data)
        {
            for (unsigned int i = 0; i < D; ++i)
                Data[i] = data;
        }

        template<typename T>
        Array<T,0>::Array()
        {
            _size = 0;
            Data = NULL;
        }

        template<typename T>
        Array<T,0>::Array(unsigned int size)
        {
            _size = size;
            Data = new T[size];
        }

        template<typename T>
        Array<T,0>::~Array()
        {
            if (Data != NULL)
                delete[] Data;
        }

        template<typename T>
        Array<T,0>::Array(const Array &a)
        {
            _size = a.Size();
            Data = new T[_size];
            Utils::Memcpy(Data, a.Data, _size);
        }

        template<typename T>
        template<typename U, unsigned int D2>
        Array<T,0>::Array(const Array<U,D2> &a)
        {
            _size = a.Size();
            Data = new T[_size];
            Utils::Memcpy(Data, a.Data, _size);
        }

        template<typename T>
        Array<T,0> &Array<T,0>::operator = (const Array<T,0> &a)
        {
            if (Data != NULL && _size != a._size) // delete si la taille est differente
                delete[] Data;
            if (Data == NULL || _size != a._size) // malloc si la taille est differente
                Data = new T[a._size];
            _size = a.Size();
            Utils::Memcpy(Data, a.Data, _size);
            return *this;
        }

        template<typename T>
        template<typename U, unsigned int D2>
        Array<T,0> &Array<T,0>::operator = (const Array<U,D2> &a)
        {
            _size = a.Size();
            if (Data != NULL)
                delete[] Data;
            Data = new T[_size];
            Utils::Memcpy(Data, a.Data, _size);
            return *this;
        }

        template<typename T>
        void Array<T,0>::InitSize(unsigned int s)
        {
            if (s != _size)
            {
                _size = s;
                if (Data != NULL)
                    delete[] Data;
                if (_size > 0)
                    Data = new T[_size];
                else
                    Data = NULL;
            }
        }

        template<typename T>
        void Array<T,0>::UnderSize(unsigned int s)
        {
            if (s < _size)
                _size = s;
        }

        template<typename T>
        void Array<T,0>::InitData(const T &data) // init de la data
        {
            for (unsigned int i = 0; i < _size; ++i)
                Data[i] = data;
        }

        template<typename T>
        void Array<T,0>::InitData(const T *data) // init de la data avec un array
        {
            Utils::Memcpy(Data, data, _size);
        }

        template<typename T>
        void Array<T,0>::Resize(unsigned int s)
        {
            T *oldData = Data;

            Data = new T[s];
            if (Data != NULL)
            {
                Utils::Memcpy(Data, oldData, ((s <= _size) ? s : _size));
                delete[] oldData;
            }
            _size = s;
        }

        template<typename T>
        T &Array<T,0>::operator [] (unsigned int i)
        {
            if (i >= _size)
                throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
            return Data[i];
        }

        template<typename T>
        const T &Array<T,0>::operator [] (unsigned int i) const
        {
            if (i >= _size)
                throw Utils::Exception("Math::Array", "Overflow in the operator[] of class Array");
            return Data[i];
        }
    }
}

#endif
