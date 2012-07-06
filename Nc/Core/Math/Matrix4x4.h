
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

#ifndef NC_CORE_MATH_MATRIX4X4_H_
#define NC_CORE_MATH_MATRIX4X4_H_

#include <iostream>
#include "Matrix.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "../Utils/Convert.h"

namespace Nc
{
    namespace Math
    {
        /// To manipulate a matrix of 4 line and 4 row type T and Manipulate basic transformation (rotation/translation/scale)
        template<typename T>
        class Matrix4x4 : public Matrix<T,4,4>
        {
            public:
                Matrix4x4()     {SetIdentity();}

            // Tests
                bool        operator == (const Matrix4x4& m) const;
                bool        operator != (const Matrix4x4& m) const;
                inline bool IsIdentity()    {return _isIdentity;}

            // Modifications
                Matrix4x4   &operator *= (const Matrix4x4 &m);
                Matrix4x4   operator * (const Matrix4x4 &m) const;

                /** Initilize the matrix */
                void Init()                                                                         {if (!_isIdentity) SetIdentity();}
                /** Set the matrix to the identity */
                void SetIdentity();
                /** Set to the projection matrix for rendering */
                void SetProjection(const T &ratioAspect, const T &nearv, const T &farv, const T &fieldOfView);
                /** Set the view matrix with eye properties */
                void SetLookAt(const Vector3D<T> &eye, const Vector3D<T> &center, const Vector3D<T> &up);
                /** Set the orthographic matrix */
                void SetOrtho(const T &left, const T &right, const T &bottom, const T &top, const T &nearVal, const T &farVal);

                /** Build the invert matrix */
                void Inverse();
                /** Exchange the lines of the matrix */
                void ExchangeXY();
                /** Exchange the lines of the matrix */
                void ExchangeYZ();
                /** Exchange the lines of the matrix */
                void ExchangeXZ();

                /** Exchange the lines of the transpose matrix */
                void TExchangeXY();
                /** Exchange the lines of the transpose matrix */
                void TExchangeYZ();
                /** Exchange the lines of the transpose matrix */
                void TExchangeXZ();

                /** Set to the Translation matrix */
                void Translation(const Vector3D<T> &v);
                /** Set to the Translation matrix */
                void Translation(const Vector2D<T> &v);
                /** Set to the Translation matrix */
                void Translation(const T &X, const T &Y, const T &Z);
                /** Set to the Translation matrix */
                void TranslationX(const T &X);
                /** Set to the Translation matrix */
                void TranslationY(const T &Y);
                /** Set to the Translation matrix */
                void TranslationZ(const T &Z);

                /** Set to the Transpose Translation */
                void TTranslation(const Vector3D<T> &v);
                /** Set to the Transpose Translation */
                void TTranslation(const Vector2D<T> &v);
                /** Set to the Transpose Translation */
                void TTranslation(const T &X, const T &Y, const T &Z);
                /** Set to the Transpose Translation */
                void TTranslationX(const T &X);
                /** Set to the Transpose Translation */
                void TTranslationY(const T &Y);
                /** Set to the Transpose Translation */
                void TTranslationZ(const T &Z);

                /** Set to the Scale matrix transformation */
                void Scale(const Vector3D<T> &v);
                /** Set to the Scale matrix transformation */
                void Scale(const T &X, const T &Y, const T &Z);

                /** Set to the Rotation matrix with an axe, angle and a bool to know if the angle is gived in radian */
                void Rotation(const Vector3D<T> &axe, float angle, bool rad = true);
                /** Set to the Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void RotationX(float angle, bool rad = true);
                /** Set to the Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void RotationY(float angle, bool rad = true);
                /** Set to the Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void RotationZ(float angle, bool rad = true);

                /** Set to the Transpose Rotation matrix with an axe, angle and a bool to know if the angle is gived in radian */
                void TRotation(const Vector3D<T> &axe, float angle, bool rad = true);
                /** Set to the Transpose Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void TRotationX(float angle, bool rad = true);
                /** Set to the Transpose Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void TRotationY(float angle, bool rad = true);
                /** Set to the Transpose Rotation matrix with an angle and a bool to know if the angle is gived in radian */
                void TRotationZ(float angle, bool rad = true);

                /** Add a transformation from an other matrix */
                void AddTransformation(const Matrix4x4& m);
                /** Add a transpose transformation from an other matrix */
                void AddTTransformation(const Matrix4x4& m);

                /** Add a Translation to the matrix */
                void AddTranslation(const Vector3D<T> &v);
                /** Add a Translation to the matrix */
                void AddTranslation(const Vector2D<T> &v);
                /** Add a Translation to the matrix */
                void AddTranslation(const T &X, const T &Y, const T &Z);

                /** Add a transpose Translation to the matrix */
                void AddTTranslation(const Vector3D<T> &v);
                /** Add a transpose Translation to the matrix */
                void AddTTranslation(const Vector2D<T> &v);
                /** Add a transpose Translation to the matrix */
                void AddTTranslation(const T &X, const T &Y, const T &Z);

                /** Add a Scale to the matrix */
                void AddScale(const Vector3D<T> &v);
                /** Add a Scale to the matrix */
                void AddScale(const T &x, const T &y, const T &z);
                /** Add a Scale to the matrix */
                void AddScale(const T &s);

                /** Add a Rotation to the matrix */
                void AddRotation(const Vector3D<T> &axe, float Angle, bool rad = true);
                /** Add a transpose Rotation to the matrix */
                void AddTRotation(const Vector3D<T> &axe, float Angle, bool rad = true);

            // transformations of basic types
                /** Transform the basic type gived in parameter */
                void Transform(Vector<T,3> &v) const;
                /** Transform the basic type gived in parameter */
                void Transform(T &x, T &y, T &z) const;
                /** Transform the basic type gived in parameter */
                void Transform(T xyz[3]) const;
                /** Transform the basic type gived in parameter */
                void Transform(Vector<T,4> &v) const;
                /** Transform the basic type gived in parameter */
                void Transform(T &x, T &y, T &z, T &u) const;

            // constantes
                static const Matrix4x4 Identity;                            ///< static const Identity matrix

            private:
                void ExchangeLine(unsigned char i, unsigned char j);
                void ExchangeColumn(unsigned char i, unsigned char j);

                bool        _isIdentity;        ///< set to true if the matrix is the identity matrix
        };

        template<typename T>
        const Matrix4x4<T> Matrix4x4<T>::Identity;

        template<typename T>
        bool        Matrix4x4<T>::operator == (const Matrix4x4& m) const
        {
            for (unsigned char i = 0; i < Matrix<T,4,4>::_sizeMax; ++i)
                if (Matrix<T,4,4>::_data[i] != m._data[i])
                    return false;
            return true;
        }

        template<typename T>
        bool        Matrix4x4<T>::operator != (const Matrix4x4& m) const
        {
            for (unsigned char i = 0; i < Matrix<T,4,4>::_sizeMax; ++i)
                if (Matrix<T,4,4>::_data[i] == m._data[i])
                    return false;
            return true;
        }

        template<typename T>
        Matrix4x4<T>   &Matrix4x4<T>::operator *= (const Matrix4x4<T> &m)
        {
            #define A(col,row) Matrix<T,4,4>::_data[(col*4)+row]
            #define B(col,row) m._data[(col*4)+row]
            for (int i = 0; i < 4; i++)
            {
                const T ai0 = A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
                A(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
                A(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
                A(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
                A(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
            }
            #undef A
            #undef B
            _isIdentity = false;
            return (*this);
        }

        template<typename T>
        Matrix4x4<T>    Matrix4x4<T>::operator * (const Matrix4x4<T> &m) const
        {
            #define A(col,row) Matrix<T,4,4>::_data[(col*4)+row]
            #define B(col,row) m._data[(col*4)+row]
            #define R(col,row) r._data[(col*4)+row]
            Matrix4x4<T>    r;
            for (int i = 0; i < 4; i++)
            {
                const T ai0 = A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
                R(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
                R(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
                R(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
                R(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
            }
            #undef A
            #undef B
            #undef R
            r._isIdentity = false;
            return r;
        }

        template<typename T>
        void Matrix4x4<T>::SetIdentity()
        {
            unsigned char k = 0;

            for (unsigned char i = 0; i < 4; ++i)
                for (unsigned char j = 0; j < 4; ++j, ++k)
                    Matrix<T,4,4>::_data[k] = (i == j) ? 1 : 0;
            _isIdentity = true;
        }

        template<typename T>
        void Matrix4x4<T>::SetProjection(const T &ratioAspect, const T &nearv, const T &farv, const T &fieldOfView)
        {
			T maxY = tanf(fieldOfView * (M_PI / 360.f)) * nearv;
            T minY = -maxY;
            T minX = minY * ratioAspect;
            T maxX = maxY * ratioAspect;

			Matrix<T,4,4>::_data[0] = 2.f * (nearv / (maxX - minX));
			Matrix<T,4,4>::_data[1] = 0;
			Matrix<T,4,4>::_data[2] = 0;
			Matrix<T,4,4>::_data[3] = 0;

			Matrix<T,4,4>::_data[4] = 0;
			Matrix<T,4,4>::_data[5] = 2.f * (nearv / (maxY - minY));
			Matrix<T,4,4>::_data[6] = 0;
			Matrix<T,4,4>::_data[7] = 0;

			Matrix<T,4,4>::_data[8] = 0;
			Matrix<T,4,4>::_data[9] = 0;
			Matrix<T,4,4>::_data[10] = -(farv + nearv) / (farv - nearv);
			Matrix<T,4,4>::_data[11] = -(2.f * farv * nearv) / (farv - nearv);

			Matrix<T,4,4>::_data[12] = 0;
			Matrix<T,4,4>::_data[13] = 0;
			Matrix<T,4,4>::_data[14] = -1.f;
			Matrix<T,4,4>::_data[15] = 0;
			_isIdentity = false;
        }

        // from the mesa sources of gluLookAt
        template<typename T>
        void Matrix4x4<T>::SetLookAt(const Vector3D<T> &eye, const Vector3D<T> &center, const Vector3D<T> &up)
        {
            Vector3D<T>     vForward(center.data[0] - eye.data[0], center.data[1] - eye.data[1], center.data[2] - eye.data[2]);
            Vector3D<T>     vSide, vUp;

            vForward.Normalize();

            // Side = forward x up
            vForward.Cross(up, vSide);
            vSide.Normalize();

            // Recompute up as: up = side x forward
            vSide.Cross(vForward, vUp);

            #define M(col,row) Matrix<T,4,4>::_data[(row*4)+col]
            M(0, 0) = vSide.data[0];
            M(1, 0) = vSide.data[1];
            M(2, 0) = vSide.data[2];
            M(3, 0) = 0;

            M(0, 1) = vUp.data[0];
            M(1, 1) = vUp.data[1];
            M(2, 1) = vUp.data[2];
            M(3, 1) = 0;

            M(0, 2) = -vForward.data[0];
            M(1, 2) = -vForward.data[1];
            M(2, 2) = -vForward.data[2];
            M(3, 2) = 0;

            M(0, 3) = 0;
            M(1, 3) = 0;
            M(2, 3) = 0;
            M(3, 3) = 1;
            #undef M

            // ajoute la translation de l'eye
            #define M(row,col) Matrix<T,4,4>::_data[(row*4)+col]
                M(0,3) = M(0,0) * -eye.data[0] + M(0,1) * -eye.data[1] + M(0,2) * -eye.data[2];
                M(1,3) = M(1,0) * -eye.data[0] + M(1,1) * -eye.data[1] + M(1,2) * -eye.data[2];
                M(2,3) = M(2,0) * -eye.data[0] + M(2,1) * -eye.data[1] + M(2,2) * -eye.data[2];
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::SetOrtho(const T &left, const T &right, const T &bottom, const T &top, const T &nearVal, const T &farVal)
        {
            #define M(row,col) Matrix<T,4,4>::_data[(row*4)+col]
            M(0,0) = 2.f / (right - left);
            M(0,1) = 0;
            M(0,2) = 0;
            M(0,3) = -((right + left) / (right - left)); // tx

            M(1,0) = 0;
            M(1,1) = 2.f / (top - bottom);
            M(1,2) = 0;
            M(1,3) = -((top + bottom) / (top - bottom)); // ty

            M(2,0) = 0;
            M(2,1) = 0;
            M(2,2) = -2.f / (farVal - nearVal);
            M(2,3) = -((farVal + nearVal) / (farVal - nearVal)); // tz

            M(3,0) = 0;
            M(3,1) = 0;
            M(3,2) = 0;
            M(3,3) = 1;
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Inverse()
        {
        // /!\ vu que ce sont que des matrices de transformation, on peut faire
            // transposée de la rotation
            Swap(Matrix<T,4,4>::_data[1], Matrix<T,4,4>::_data[1*4]);
            Swap(Matrix<T,4,4>::_data[2], Matrix<T,4,4>::_data[2*4]),
            Swap(Matrix<T,4,4>::_data[(1*4) + 2], Matrix<T,4,4>::_data[(2*4) + 1]);

            // inverse de la translation
            Matrix<T,4,4>::_data[(0*4) + 3] = -Matrix<T,4,4>::_data[(0*4) + 3];
            Matrix<T,4,4>::_data[(1*4) + 3] = -Matrix<T,4,4>::_data[(1*4) + 3];
            Matrix<T,4,4>::_data[(2*4) + 3] = -Matrix<T,4,4>::_data[(2*4) + 3];

            // multiplication
            double tmp1 =   Matrix<T,4,4>::_data[0] * Matrix<T,4,4>::_data[3] +
                            Matrix<T,4,4>::_data[1] * Matrix<T,4,4>::_data[(1*4)+3] +
                            Matrix<T,4,4>::_data[2] * Matrix<T,4,4>::_data[(2*4)+3];
            double tmp2 =   Matrix<T,4,4>::_data[4] * Matrix<T,4,4>::_data[3] +
                            Matrix<T,4,4>::_data[(1*4)+1] * Matrix<T,4,4>::_data[(1*4)+3] +
                            Matrix<T,4,4>::_data[(1*4)+2] * Matrix<T,4,4>::_data[(2*4)+3];

            Matrix<T,4,4>::_data[(2*4)+3] = Matrix<T,4,4>::_data[2*4] * Matrix<T,4,4>::_data[3] +
                                            Matrix<T,4,4>::_data[(2*4)+1] * Matrix<T,4,4>::_data[(1*4)+3] +
                                            Matrix<T,4,4>::_data[(2*4)+2] * Matrix<T,4,4>::_data[(2*4)+3];
            Matrix<T,4,4>::_data[3] = tmp1;
            Matrix<T,4,4>::_data[(1*4)+3] = tmp2;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::ExchangeXY()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeLine(0,1);
        }

        template<typename T>
        void Matrix4x4<T>::ExchangeYZ()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeLine(1,2);
        }

        template<typename T>
        void Matrix4x4<T>::ExchangeXZ()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeLine(0,2);
        }

        template<typename T>
        void Matrix4x4<T>::TExchangeXY()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeColumn(0,1);
        }

        template<typename T>
        void Matrix4x4<T>::TExchangeYZ()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeColumn(1,2);
        }

        template<typename T>
        void Matrix4x4<T>::TExchangeXZ()
        {
            if (!_isIdentity)
                SetIdentity();
            ExchangeColumn(0,2);
        }


        template<typename T>
        void Matrix4x4<T>::Translation(const Vector3D<T> &v)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(0 * 4) + 3] = v.data[0];
            Matrix<T,4,4>::_data[(1 * 4) + 3] = v.data[1];
            Matrix<T,4,4>::_data[(2 * 4) + 3] = v.data[2];
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Translation(const Vector2D<T> &v)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(0 * 4) + 3] = v.data[0];
            Matrix<T,4,4>::_data[(1 * 4) + 3] = v.data[1];
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Translation(const T &X, const T &Y, const T &Z)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(0 * 4) + 3] = X;
            Matrix<T,4,4>::_data[(1 * 4) + 3] = Y;
            Matrix<T,4,4>::_data[(2 * 4) + 3] = Z;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TranslationX(const T &X)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(0 * 4) + 3] = X;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TranslationY(const T &Y)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(1 * 4) + 3] = Y;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TranslationZ(const T &Z)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(2 * 4) + 3] = Z;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslation(const Vector3D<T> &v)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 0] = v.data[0];
            Matrix<T,4,4>::_data[(3 * 4) + 1] = v.data[1];
            Matrix<T,4,4>::_data[(3 * 4) + 2] = v.data[2];
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslation(const Vector2D<T> &v)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 0] = v.data[0];
            Matrix<T,4,4>::_data[(3 * 4) + 1] = v.data[1];
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslation(const T &X, const T &Y, const T &Z)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 0] = X;
            Matrix<T,4,4>::_data[(3 * 4) + 1] = Y;
            Matrix<T,4,4>::_data[(3 * 4) + 2] = Z;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslationX(const T &X)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 0] = X;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslationY(const T &Y)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 1] = Y;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TTranslationZ(const T &Z)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(3 * 4) + 3] = Z;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Scale(const Vector3D<T> &v)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = v.data[0];
            Matrix<T,4,4>::_data[(1 * 4) + 1] = v.data[1];
            Matrix<T,4,4>::_data[(2 * 4) + 2] = v.data[2];
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Scale(const T &X, const T &Y, const T &Z)
        {
            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = X;
            Matrix<T,4,4>::_data[(1 * 4) + 1] = Y;
            Matrix<T,4,4>::_data[(2 * 4) + 2] = Z;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::Rotation(const Vector3D<T> &axe, float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] =           (axe.data[0] * axe.data[0] * (1-Cos)) + Cos;
            Matrix<T,4,4>::_data[1] =           (axe.data[0] * axe.data[1] * (1-Cos)) - (axe.data[2] * Sin);
            Matrix<T,4,4>::_data[2] =           (axe.data[0] * axe.data[2] * (1-Cos)) + (axe.data[1] * Sin);
            Matrix<T,4,4>::_data[(1 * 4) + 0] = (axe.data[1] * axe.data[0] * (1-Cos)) + (axe.data[2] * Sin);
            Matrix<T,4,4>::_data[(1 * 4) + 1] = (axe.data[1] * axe.data[1] * (1-Cos)) + Cos;
            Matrix<T,4,4>::_data[(1 * 4) + 2] = (axe.data[1] * axe.data[2] * (1-Cos)) - (axe.data[0] * Sin);
            Matrix<T,4,4>::_data[(2 * 4) + 0] = (axe.data[2] * axe.data[0] * (1-Cos)) - (axe.data[1] * Sin);
            Matrix<T,4,4>::_data[(2 * 4) + 1] = (axe.data[2] * axe.data[1] * (1-Cos)) + (axe.data[0] * Sin);
            Matrix<T,4,4>::_data[(2 * 4) + 2] = (axe.data[2] * axe.data[2] * (1-Cos)) + Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::RotationX(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(1 * 4) + 1] = Cos;
            Matrix<T,4,4>::_data[(1 * 4) + 2] = -Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 1] = Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 2] = Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::RotationY(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = Cos;
            Matrix<T,4,4>::_data[2] = Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 0] = -Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 2] = Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::RotationZ(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = Cos;
            Matrix<T,4,4>::_data[1] = -Sin;
            Matrix<T,4,4>::_data[1 * 4] = Sin;
            Matrix<T,4,4>::_data[(1 * 4) + 1] = Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TRotation(const Vector3D<T> &axe, float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] =           (axe.data[0] * axe.data[0] * (1-Cos)) + Cos;
            Matrix<T,4,4>::_data[(1 * 4) + 1] = (axe.data[0] * axe.data[1] * (1-Cos)) - (axe.data[2] * Sin);
            Matrix<T,4,4>::_data[(2 * 4) + 2] = (axe.data[0] * axe.data[2] * (1-Cos)) + (axe.data[1] * Sin);
            Matrix<T,4,4>::_data[(1 * 4) + 0] = (axe.data[1] * axe.data[0] * (1-Cos)) + (axe.data[2] * Sin);
            Matrix<T,4,4>::_data[1] =           (axe.data[1] * axe.data[1] * (1-Cos)) + Cos;
            Matrix<T,4,4>::_data[(2 * 4) + 1] = (axe.data[1] * axe.data[2] * (1-Cos)) - (axe.data[0] * Sin);
            Matrix<T,4,4>::_data[(2 * 4) + 0] = (axe.data[2] * axe.data[0] * (1-Cos)) - (axe.data[1] * Sin);
            Matrix<T,4,4>::_data[(1 * 4) + 2] = (axe.data[2] * axe.data[1] * (1-Cos)) + (axe.data[0] * Sin);
            Matrix<T,4,4>::_data[2] =           (axe.data[2] * axe.data[2] * (1-Cos)) + Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TRotationX(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[(1 * 4) + 1] = Cos;
            Matrix<T,4,4>::_data[(2 * 4) + 1] = -Sin;
            Matrix<T,4,4>::_data[(1 * 4) + 2] = Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 2] = Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TRotationY(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = Cos;
            Matrix<T,4,4>::_data[(2 * 4) + 0] = Sin;
            Matrix<T,4,4>::_data[2] = -Sin;
            Matrix<T,4,4>::_data[(2 * 4) + 2] = Cos;
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::TRotationZ(float angle, bool rad)
        {
            if(!rad)
                angle = Utils::Convert::DegToRad(angle);

            float Cos = cos(angle), Sin = sin(angle);

            if (!_isIdentity)
                SetIdentity();
            Matrix<T,4,4>::_data[0] = Cos;
            Matrix<T,4,4>::_data[1 * 4] = -Sin;
            Matrix<T,4,4>::_data[1] = Sin;
            Matrix<T,4,4>::_data[(1 * 4) + 1] = Cos;
            _isIdentity = false;
        }


        template<typename T>
        void Matrix4x4<T>::AddTransformation(const Matrix4x4& m)
        {
            (*this) = m * (*this);
        }

        template<typename T>
        void Matrix4x4<T>::AddTTransformation(const Matrix4x4& m)
        {
            Matrix4x4<T> m2;
            m.Transpose(m2);
            (*this) = m2 * (*this);
        }

        template<typename T>
        void Matrix4x4<T>::AddTranslation(const Vector3D<T> &v)
        {
            #define M(row,col)  Matrix<T,4,4>::_data[(row*4) + col]
            M(0,3) += v.data[0];
            M(1,3) += v.data[1];
            M(2,3) += v.data[2];
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::AddTranslation(const Vector2D<T> &v)
        {
            #define M(row,col)  Matrix<T,4,4>::_data[(row*4) + col]
            M(0,3) += v.data[0];
            M(1,3) += v.data[1];
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::AddTranslation(const T &X, const T &Y, const T &Z)
        {
            #define M(row,col)  Matrix<T,4,4>::_data[(row*4) + col]
            M(0,3) += X;
            M(1,3) += Y;
            M(2,3) += Z;
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::AddTTranslation(const Vector3D<T> &v)
        {
            AddTTranslation(v.data[0], v.data[1], v.data[2]);
        }

        template<typename T>
        void Matrix4x4<T>::AddTTranslation(const Vector2D<T> &v)
        {
            AddTTranslation(v.data[0], v.data[1], 0);
        }

        template<typename T>
        void Matrix4x4<T>::AddTTranslation(const T &X, const T &Y, const T &Z)
        {
            #define M(row,col)  Matrix<T,4,4>::_data[(row*4) + col]
            M(3,0) += M(0,0) * X + M(1,0) * Y + M(2,0) * Z;
            M(3,1) += M(0,1) * X + M(1,1) * Y + M(2,1) * Z;
            M(3,2) += M(0,2) * X + M(1,2) * Y + M(2,2) * Z;
            M(3,3) += M(0,3) * X + M(1,3) * Y + M(2,3) * Z;
            #undef M
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::AddScale(const Vector3D<T> &v)
        {
            Matrix4x4<T> m;
            m.Scale(v);
            AddTransformation(m);
        }

        template<typename T>
        void Matrix4x4<T>::AddScale(const T &x, const T &y, const T &z)
        {
            Matrix4x4<T> m;
            m.Scale(x,y,z);
            AddTransformation(m);
        }

        template<typename T>
        void Matrix4x4<T>::AddScale(const T &s)
        {
            Matrix4x4<T> m;
            m.Scale(s, s, s);
            AddTransformation(m);
        }

        template<typename T>
        void Matrix4x4<T>::AddRotation(const Vector3D<T> &axe, float angle, bool rad)
        {
            Matrix4x4<T> m;
            m.Rotation(axe, angle, rad);
            AddTransformation(m);
        }

        template<typename T>
        void Matrix4x4<T>::AddTRotation(const Vector3D<T> &axe, float angle, bool rad)
        {
            Matrix4x4<T> m;
            m.TRotation(axe, angle, rad);
            AddTransformation(m);
        }

        template<typename T>
        void Matrix4x4<T>::Transform(Vector<T,3> &v) const
        {
            Transform(v.data[0], v.data[1], v.data[2]);
        }

        template<typename T>
        void Matrix4x4<T>::Transform(Vector<T,4> &v) const
        {
            Transform(v.data[0], v.data[1], v.data[2], v.data[3]);
        }

        template<typename T>
        void Matrix4x4<T>::Transform(T xyz[3]) const
        {
            Transform(xyz[0], xyz[1], xyz[2]);
        }

        template<typename T>
        void Matrix4x4<T>::Transform(T &x, T &y, T &z) const
        {
            T elem[3];
            for(unsigned short i = 0; i < 3; ++i)
            {
                elem[i] =   x * Matrix<T,4,4>::_data[i*4] +
                            y * Matrix<T,4,4>::_data[(i*4)+1] +
                            z * Matrix<T,4,4>::_data[(i*4)+2] +
                            Matrix<T,4,4>::_data[(i*4)+3];
            }
            x = elem[0];
            y = elem[1];
            z = elem[2];
        }

        template<typename T>
        void Matrix4x4<T>::Transform(T &x, T &y, T &z, T &u) const
        {
            T elem[4];
            for (unsigned int i = 0; i < 4; ++i)
            {
                elem[i] =   x * Matrix<T,4,4>::_data[i*4] +
                            y * Matrix<T,4,4>::_data[(i*4)+1] +
                            z * Matrix<T,4,4>::_data[(i*4)+2] +
                            u * Matrix<T,4,4>::_data[(i*4)+3];
            }
            x = elem[0];
            y = elem[1];
            z = elem[2];
            u = elem[3];
        }

        template<typename T>
        void Matrix4x4<T>::ExchangeLine(unsigned char i, unsigned char j)
        {
            Swap(Matrix<T,4,4>::_data[(i * 4) + 0], Matrix<T,4,4>::_data[(j * 4) + 0]);
            Swap(Matrix<T,4,4>::_data[(i * 4) + 1], Matrix<T,4,4>::_data[(j * 4) + 1]);
            Swap(Matrix<T,4,4>::_data[(i * 4) + 2], Matrix<T,4,4>::_data[(j * 4) + 2]);
            Swap(Matrix<T,4,4>::_data[(i * 4) + 3], Matrix<T,4,4>::_data[(j * 4) + 3]);
            _isIdentity = false;
        }

        template<typename T>
        void Matrix4x4<T>::ExchangeColumn(unsigned char i, unsigned char j)
        {
            Swap(Matrix<T,4,4>::_data[(0 * 4) + i], Matrix<T,4,4>::_data[(0 * 4) + j]);
            Swap(Matrix<T,4,4>::_data[(1 * 4) + i], Matrix<T,4,4>::_data[(1 * 4) + j]);
            Swap(Matrix<T,4,4>::_data[(2 * 4) + i], Matrix<T,4,4>::_data[(2 * 4) + j]);
            Swap(Matrix<T,4,4>::_data[(3 * 4) + i], Matrix<T,4,4>::_data[(3 * 4) + j]);
            _isIdentity = false;
        }
    }
}

#endif
