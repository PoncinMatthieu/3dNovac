
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "MatrixAnimation.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace Nc;
using namespace Nc::Graphic;

MatrixAnimation::MatrixAnimation()
{
    _alive = true;
    _object = NULL;
}

MatrixAnimation::MatrixAnimation(Object *o, const TMatrix &transformation)
{
    _alive = true;
    _object = o;
//    _transformation = transformation;
    _initialMatrix = _object->Matrix;
}

MatrixAnimation::~MatrixAnimation()
{
}

void MatrixAnimation::Update(float runningTime)
{
    if (_object != NULL)
    {
//TODO, trouver un moyen d'appliquer le facteur temp a la matrice
/*
        mMatrice m(_transformation);
        for (int i = 0; i < 16; i++)
        {
            m.Element()[i] *= runningTime * 0.1;
        }
        _object->Matrice().AddTransformation(m);
*/
        _object->Matrix.AddRotation(_rotation, _angle * runningTime);
    }
}
